/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: xcp_damper.c
 *
 * Code generated for Simulink model 'xcp_damper'.
 *
 * Model version                  : 1.6
 * Simulink Coder version         : 23.2 (R2023b) 01-Aug-2023
 * C/C++ source code generated on : Thu Apr  2 13:43:04 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "xcp_damper.h"
#include <math.h>
#include "rtwtypes.h"
#include "xcp_damper_private.h"

/* Block signals (default storage) */
B_xcp_damper_T xcp_damper_B;

/* Block states (default storage) */
DW_xcp_damper_T xcp_damper_DW;

/* External outputs (root outports fed by signals with default storage) */
ExtY_xcp_damper_T xcp_damper_Y;

/* Real-time model */
static RT_MODEL_xcp_damper_T xcp_damper_M_;
RT_MODEL_xcp_damper_T *const xcp_damper_M = &xcp_damper_M_;

/* Model step function */
void xcp_damper_step(void)
{
  /* DiscreteIntegrator: '<Root>/Discrete-Time Integrator1' */
  xcp_damper_B.x = xcp_damper_DW.DiscreteTimeIntegrator1_DSTATE;

  /* DiscreteIntegrator: '<Root>/Discrete-Time Integrator' */
  xcp_damper_B.v = xcp_damper_DW.DiscreteTimeIntegrator_DSTATE;

  /* Gain: '<Root>/1//Mass ' incorporates:
   *  Gain: '<Root>/Damping'
   *  Gain: '<Root>/Stiffness'
   *  Sin: '<Root>/Sine Wave'
   *  Sum: '<Root>/Add'
   *  Sum: '<Root>/Sum1'
   */
  xcp_damper_B.a = ((0.0 - (sin((real_T)xcp_damper_DW.counter * 2.0 *
    3.1415926535897931 / 10.0) * 10.0 + 400.0 * xcp_damper_B.x)) - 10.0 *
                    xcp_damper_B.v) * 0.27777777777777779;

  /* Outport: '<Root>/Out1' */
  xcp_damper_Y.Out1[0] = xcp_damper_B.x;
  xcp_damper_Y.Out1[1] = xcp_damper_B.v;
  xcp_damper_Y.Out1[2] = xcp_damper_B.a;

  /* Update for DiscreteIntegrator: '<Root>/Discrete-Time Integrator1' */
  xcp_damper_DW.DiscreteTimeIntegrator1_DSTATE += 0.01 * xcp_damper_B.v;

  /* Update for Sin: '<Root>/Sine Wave' */
  xcp_damper_DW.counter++;
  if (xcp_damper_DW.counter == 10) {
    xcp_damper_DW.counter = 0;
  }

  /* End of Update for Sin: '<Root>/Sine Wave' */

  /* Update for DiscreteIntegrator: '<Root>/Discrete-Time Integrator' */
  xcp_damper_DW.DiscreteTimeIntegrator_DSTATE += 0.01 * xcp_damper_B.a;

  {                                    /* Sample time: [0.01s, 0.0s] */
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   */
  xcp_damper_M->Timing.taskTime0 =
    ((time_T)(++xcp_damper_M->Timing.clockTick0)) *
    xcp_damper_M->Timing.stepSize0;
}

/* Model initialize function */
void xcp_damper_initialize(void)
{
  /* Registration code */
  rtmSetTFinal(xcp_damper_M, 10.0);
  xcp_damper_M->Timing.stepSize0 = 0.01;

  /* External mode info */
  xcp_damper_M->Sizes.checksums[0] = (543012316U);
  xcp_damper_M->Sizes.checksums[1] = (372585551U);
  xcp_damper_M->Sizes.checksums[2] = (2155171695U);
  xcp_damper_M->Sizes.checksums[3] = (1626101893U);

  {
    static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE;
    static RTWExtModeInfo rt_ExtModeInfo;
    static const sysRanDType *systemRan[1];
    xcp_damper_M->extModeInfo = (&rt_ExtModeInfo);
    rteiSetSubSystemActiveVectorAddresses(&rt_ExtModeInfo, systemRan);
    systemRan[0] = &rtAlwaysEnabled;
    rteiSetModelMappingInfoPtr(xcp_damper_M->extModeInfo,
      &xcp_damper_M->SpecialInfo.mappingInfo);
    rteiSetChecksumsPtr(xcp_damper_M->extModeInfo, xcp_damper_M->Sizes.checksums);
    rteiSetTPtr(xcp_damper_M->extModeInfo, rtmGetTPtr(xcp_damper_M));
  }
}

/* Model terminate function */
void xcp_damper_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
