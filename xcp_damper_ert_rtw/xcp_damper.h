/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: xcp_damper.h
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

#ifndef RTW_HEADER_xcp_damper_h_
#define RTW_HEADER_xcp_damper_h_
#ifndef xcp_damper_COMMON_INCLUDES_
#define xcp_damper_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_extmode.h"
#include "sysran_types.h"
#endif                                 /* xcp_damper_COMMON_INCLUDES_ */

#include "xcp_damper_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetFinalTime
#define rtmGetFinalTime(rtm)           ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
#define rtmGetStopRequested(rtm)       ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
#define rtmSetStopRequested(rtm, val)  ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
#define rtmGetStopRequestedPtr(rtm)    (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
#define rtmGetT(rtm)                   ((rtm)->Timing.taskTime0)
#endif

#ifndef rtmGetTFinal
#define rtmGetTFinal(rtm)              ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                (&(rtm)->Timing.taskTime0)
#endif

/* Block signals (default storage) */
typedef struct {
  real_T x;                            /* '<Root>/Discrete-Time Integrator1' */
  real_T v;                            /* '<Root>/Discrete-Time Integrator' */
  real_T a;                            /* '<Root>/1//Mass ' */
} B_xcp_damper_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T DiscreteTimeIntegrator1_DSTATE;/* '<Root>/Discrete-Time Integrator1' */
  real_T DiscreteTimeIntegrator_DSTATE;/* '<Root>/Discrete-Time Integrator' */
  struct {
    void *LoggedData;
  } VelocityofMass_PWORK;              /* '<Root>/Velocity of Mass' */

  int32_T counter;                     /* '<Root>/Sine Wave' */
} DW_xcp_damper_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T Out1[3];                      /* '<Root>/Out1' */
} ExtY_xcp_damper_T;

/* Real-time Model Data Structure */
struct tag_RTM_xcp_damper_T {
  const char_T *errorStatus;
  RTWExtModeInfo *extModeInfo;

  /*
   * Sizes:
   * The following substructure contains sizes information
   * for many of the model attributes such as inputs, outputs,
   * dwork, sample times, etc.
   */
  struct {
    uint32_T checksums[4];
  } Sizes;

  /*
   * SpecialInfo:
   * The following substructure contains special information
   * related to other components that are dependent on RTW.
   */
  struct {
    const void *mappingInfo;
  } SpecialInfo;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    time_T taskTime0;
    uint32_T clockTick0;
    time_T stepSize0;
    time_T tFinal;
    boolean_T stopRequestedFlag;
  } Timing;
};

/* Block signals (default storage) */
extern B_xcp_damper_T xcp_damper_B;

/* Block states (default storage) */
extern DW_xcp_damper_T xcp_damper_DW;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_xcp_damper_T xcp_damper_Y;

/* Model entry point functions */
extern void xcp_damper_initialize(void);
extern void xcp_damper_step(void);
extern void xcp_damper_terminate(void);

/* Real-time Model object */
extern RT_MODEL_xcp_damper_T *const xcp_damper_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'xcp_damper'
 * '<S1>'   : 'xcp_damper/Sensing Simulink'
 */
#endif                                 /* RTW_HEADER_xcp_damper_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
