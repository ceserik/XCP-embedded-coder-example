import argparse
import re
import socket
import struct
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, Optional, Tuple


# --- Edit these defaults at the top ---
DEFAULT_HOST = "127.0.0.1"
DEFAULT_PORT = 17725
DEFAULT_TIMEOUT = 3.0
DEFAULT_A2L = Path(__file__).resolve().parent.parent / "mymodel_ert_rtw" / "mymodel.a2l"


# XCP command codes (CRO)
CMD_CONNECT = 0xFF
CMD_DISCONNECT = 0xFE
CMD_SHORT_UPLOAD = 0xF4

# XCP packet identifiers (CTO response first byte)
PID_RES = 0xFF
PID_ERR = 0xFE


@dataclass
class A2LSymbol:
	name: str
	address: int
	datatype: Optional[str]
	kind: str


DATA_TYPE_SIZE = {
	"UBYTE": 1,
	"SBYTE": 1,
	"UWORD": 2,
	"SWORD": 2,
	"ULONG": 4,
	"SLONG": 4,
	"A_UINT64": 8,
	"A_INT64": 8,
	"FLOAT32_IEEE": 4,
	"FLOAT64_IEEE": 8,
}


def decode_value(data: bytes, datatype: Optional[str]) -> Optional[object]:
	if datatype is None:
		return None
	dt = datatype.upper()
	try:
		if dt == "UBYTE" and len(data) >= 1:
			return struct.unpack("<B", data[:1])[0]
		if dt == "SBYTE" and len(data) >= 1:
			return struct.unpack("<b", data[:1])[0]
		if dt == "UWORD" and len(data) >= 2:
			return struct.unpack("<H", data[:2])[0]
		if dt == "SWORD" and len(data) >= 2:
			return struct.unpack("<h", data[:2])[0]
		if dt == "ULONG" and len(data) >= 4:
			return struct.unpack("<I", data[:4])[0]
		if dt == "SLONG" and len(data) >= 4:
			return struct.unpack("<i", data[:4])[0]
		if dt == "A_UINT64" and len(data) >= 8:
			return struct.unpack("<Q", data[:8])[0]
		if dt == "A_INT64" and len(data) >= 8:
			return struct.unpack("<q", data[:8])[0]
		if dt == "FLOAT32_IEEE" and len(data) >= 4:
			return struct.unpack("<f", data[:4])[0]
		if dt == "FLOAT64_IEEE" and len(data) >= 8:
			return struct.unpack("<d", data[:8])[0]
	except struct.error:
		return None
	return None


def pack_xcp_tcp(payload: bytes, counter: int = 0) -> bytes:
	"""Pack XCP payload with XCP-on-TCP transport header.

	Header format (little-endian):
	  - length:  uint16
	  - counter: uint16
	"""
	return struct.pack("<HH", len(payload), counter & 0xFFFF) + payload


def recv_exact(sock: socket.socket, n: int) -> bytes:
	data = bytearray()
	while len(data) < n:
		chunk = sock.recv(n - len(data))
		if not chunk:
			raise ConnectionError("Socket closed while receiving data")
		data.extend(chunk)
	return bytes(data)


def recv_xcp_tcp(sock: socket.socket) -> Tuple[int, bytes]:
	header = recv_exact(sock, 4)
	length, counter = struct.unpack("<HH", header)
	payload = recv_exact(sock, length)
	return counter, payload


def xcp_request(sock: socket.socket, payload: bytes, counter: int) -> Tuple[int, bytes]:
	sock.sendall(pack_xcp_tcp(payload, counter=counter))
	rx_counter, rx_payload = recv_xcp_tcp(sock)
	if not rx_payload:
		raise RuntimeError("Empty XCP response")
	pid = rx_payload[0]
	if pid == PID_ERR:
		err_code = rx_payload[1] if len(rx_payload) > 1 else None
		raise RuntimeError(f"XCP ERR response, code={err_code}")
	if pid != PID_RES:
		raise RuntimeError(f"Unexpected PID in response: 0x{pid:02X}")
	return rx_counter, rx_payload


def parse_a2l_symbols(a2l_path: Path) -> Dict[str, A2LSymbol]:
	text = a2l_path.read_text(encoding="latin-1", errors="ignore")
	symbols: Dict[str, A2LSymbol] = {}
	record_layout_dtype: Dict[str, str] = {}

	for rl in re.finditer(
		r"/begin\s+RECORD_LAYOUT\s+(\S+)(.*?)/end\s+RECORD_LAYOUT",
		text,
		flags=re.IGNORECASE | re.DOTALL,
	):
		rl_name = rl.group(1)
		rl_block = rl.group(2)
		dt_match = re.search(r"FNC_VALUES\s+\d+\s+(\S+)", rl_block, flags=re.IGNORECASE)
		if dt_match:
			record_layout_dtype[rl_name] = dt_match.group(1).upper()

	# Simulink multiline format:
	# /begin MEASUREMENT
	#   /* Name */ mymodel_B.a
	#   /* Data type */ FLOAT64_IEEE
	#   ECU_ADDRESS 0x....
	# /end MEASUREMENT
	for blk in re.finditer(
		r"/begin\s+MEASUREMENT\b(.*?)/end\s+MEASUREMENT",
		text,
		flags=re.IGNORECASE | re.DOTALL,
	):
		block = blk.group(1)
		name_match = re.search(r"/\*\s*Name\s*\*/\s*(\S+)", block, flags=re.IGNORECASE)
		if not name_match:
			continue
		name = name_match.group(1)
		dtype_match = re.search(r"/\*\s*Data\s*type\s*\*/\s*(\S+)", block, flags=re.IGNORECASE)
		datatype = dtype_match.group(1).upper() if dtype_match else None
		addr_match = re.search(r"ECU_ADDRESS\s+(0x[0-9A-Fa-f]+)", block, flags=re.IGNORECASE)
		if not addr_match:
			continue
		symbols[name] = A2LSymbol(
			name=name,
			address=int(addr_match.group(1), 16),
			datatype=datatype,
			kind="MEASUREMENT",
		)

	# Inline format fallback
	for m in re.finditer(
		r"/begin\s+MEASUREMENT\s+(\S+)\s+\"[^\"]*\"\s+(\S+)(.*?)/end\s+MEASUREMENT",
		text,
		flags=re.IGNORECASE | re.DOTALL,
	):
		name = m.group(1)
		datatype = m.group(2).upper()
		block = m.group(3)
		addr_match = re.search(r"ECU_ADDRESS\s+(0x[0-9A-Fa-f]+)", block, flags=re.IGNORECASE)
		if not addr_match:
			continue
		symbols[name] = A2LSymbol(
			name=name,
			address=int(addr_match.group(1), 16),
			datatype=datatype,
			kind="MEASUREMENT",
		)

	# Simulink multiline CHARACTERISTIC format
	for blk in re.finditer(
		r"/begin\s+CHARACTERISTIC\b(.*?)/end\s+CHARACTERISTIC",
		text,
		flags=re.IGNORECASE | re.DOTALL,
	):
		block = blk.group(1)
		name_match = re.search(r"/\*\s*Name\s*\*/\s*(\S+)", block, flags=re.IGNORECASE)
		if not name_match:
			continue
		name = name_match.group(1)
		if name in symbols:
			continue
		addr_match = re.search(
			r"ECU_ADDRESS\s+(0x[0-9A-Fa-f]+)|/\*\s*ECU\s*Address\s*\*/\s*(0x[0-9A-Fa-f]+)",
			block,
			flags=re.IGNORECASE,
		)
		if not addr_match:
			continue
		addr_hex = addr_match.group(1) or addr_match.group(2)
		record_layout_match = re.search(
			r"/\*\s*Record\s*Layout\s*\*/\s*(\S+)",
			block,
			flags=re.IGNORECASE,
		)
		datatype = None
		if record_layout_match:
			datatype = record_layout_dtype.get(record_layout_match.group(1))
		symbols[name] = A2LSymbol(
			name=name,
			address=int(addr_hex, 16),
			datatype=datatype,
			kind="CHARACTERISTIC",
		)

	# Inline format fallback
	for c in re.finditer(
		r"/begin\s+CHARACTERISTIC\s+(\S+)\s+\"[^\"]*\"\s+\S+\s+(0x[0-9A-Fa-f]+)",
		text,
		flags=re.IGNORECASE,
	):
		name = c.group(1)
		if name in symbols:
			continue
		symbols[name] = A2LSymbol(
			name=name,
			address=int(c.group(2), 16),
			datatype=None,
			kind="CHARACTERISTIC",
		)

	return symbols


def short_upload(sock: socket.socket, counter: int, address: int, size_bytes: int) -> Tuple[int, bytes]:
	if not (1 <= size_bytes <= 255):
		raise ValueError("SHORT_UPLOAD size must be between 1 and 255 bytes")
	# SHORT_UPLOAD: [CMD, n, reserved, addr_ext, addr32]
	# For Simulink generated x86 target, addr_ext is typically 0x00.
	payload = bytes([CMD_SHORT_UPLOAD, size_bytes, 0x00, 0x00]) + struct.pack("<I", address)
	_, rx = xcp_request(sock, payload, counter)
	# rx: [PID_RES, data...]
	return counter + 1, rx[1:]


def main() -> int:
	parser = argparse.ArgumentParser(description="Simple XCP-over-TCP connect test")
	parser.add_argument("--host", default=DEFAULT_HOST, help="XCP server host")
	parser.add_argument("--port", type=int, default=DEFAULT_PORT, help="XCP server TCP port")
	parser.add_argument("--timeout", type=float, default=DEFAULT_TIMEOUT, help="Socket timeout in seconds")
	parser.add_argument("--linger", type=float, default=0.2, help="Seconds to wait before closing socket")
	parser.add_argument("--a2l", type=Path, default=DEFAULT_A2L, help="Path to A2L file")
	parser.add_argument("--list-symbols", action="store_true", help="List symbols parsed from A2L and exit")
	parser.add_argument("--symbol", help="A2L symbol (MEASUREMENT/CHARACTERISTIC) to read with SHORT_UPLOAD")
	parser.add_argument("--bytes", type=int, help="Read size in bytes (overrides A2L datatype size)")
	parser.add_argument("--print-all", action="store_true", help="Print all model variables and read their values")
	parser.add_argument("--allow-zero-address", action="store_true", help="Try reading symbols even when ECU_ADDRESS is 0x0000")
	args = parser.parse_args()

	symbols: Dict[str, A2LSymbol] = {}
	if args.a2l.exists():
		symbols = parse_a2l_symbols(args.a2l)
	else:
		print(f"Warning: A2L file not found: {args.a2l}")

	if args.list_symbols:
		if not symbols:
			print("No symbols parsed from A2L.")
			return 0
		for sym in sorted(symbols.values(), key=lambda s: s.name):
			dtype = sym.datatype or "-"
			print(f"{sym.name:40s} 0x{sym.address:08X} {sym.kind:14s} {dtype}")
		return 0

	# CONNECT request payload (CRO): [CMD_CONNECT, mode]
	# mode=0x00 means normal mode.
	connect_payload = bytes([CMD_CONNECT, 0x00])
	disconnect_payload = bytes([CMD_DISCONNECT])
	counter = 1

	with socket.create_connection((args.host, args.port), timeout=args.timeout) as sock:
		sock.settimeout(args.timeout)

		# Send CONNECT
		rx_counter, rx_payload = xcp_request(sock, connect_payload, counter)
		counter += 1
		print(f"Connected to XCP server at {args.host}:{args.port}")
		print(f"RX counter: {rx_counter}")
		print(f"CONNECT RES payload: {rx_payload.hex(' ')}")

		if args.print_all:
			if not symbols:
				print("No symbols parsed from A2L.")
			else:
				for sym in sorted(symbols.values(), key=lambda s: s.name):
					if sym.address == 0 and not args.allow_zero_address:
						print(f"{sym.name} @ 0x00000000 = <unresolved A2L ECU_ADDRESS>")
						continue

					size = args.bytes
					if size is None and sym.datatype is not None:
						size = DATA_TYPE_SIZE.get(sym.datatype)

					if size is None:
						print(
							f"{sym.name} @ 0x{sym.address:08X} ({sym.kind}) = <size unknown, use --bytes>"
						)
						continue

					try:
						counter, data = short_upload(sock, counter, sym.address, size)
						decoded = decode_value(data, sym.datatype)
						if decoded is not None:
							print(f"{sym.name} = {decoded}")
						else:
							print(f"{sym.name} = <cannot decode {sym.datatype or 'raw'}>")
					except Exception as exc:
						print(f"{sym.name} @ 0x{sym.address:08X} = <read failed: {exc}>")

		if args.symbol:
			sym = symbols.get(args.symbol)
			if sym is None:
				available = ", ".join(sorted(symbols.keys())[:10])
				raise RuntimeError(f"Symbol '{args.symbol}' not found in A2L. Examples: {available}")

			if sym.address == 0 and not args.allow_zero_address:
				raise RuntimeError(
					"Symbol has unresolved ECU_ADDRESS=0x00000000 in A2L. "
					"Rebuild A2L with resolved addresses or pass --allow-zero-address."
				)

			size = args.bytes
			if size is None:
				if sym.datatype is None:
					raise RuntimeError(
						"Cannot infer size for this symbol. Pass --bytes N explicitly."
					)
				size = DATA_TYPE_SIZE.get(sym.datatype)
				if size is None:
					raise RuntimeError(
						f"Unknown A2L datatype '{sym.datatype}'. Pass --bytes N explicitly."
					)

			counter, data = short_upload(sock, counter, sym.address, size)
			decoded = decode_value(data, sym.datatype)
			if decoded is not None:
				print(f"{sym.name} = {decoded}")
			else:
				print(f"{sym.name} = <cannot decode {sym.datatype or 'raw'}>")

		# Send DISCONNECT (best-effort)
		try:
			# Graceful XCP disconnect: send and wait for RES before closing socket.
			xcp_request(sock, disconnect_payload, counter)
		except Exception:
			pass

		# Give server background thread time to settle before TCP close.
		# This reduces xcpTransportRx errors on some Simulink targets.
		if args.linger > 0:
			sock.settimeout(min(args.linger, 0.5))
			try:
				recv_xcp_tcp(sock)
			except Exception:
				pass

	return 0


if __name__ == "__main__":
	raise SystemExit(main())
