/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: mymodel.c
 *
 * Code generated for Simulink model 'mymodel'.
 *
 * Model version                  : 1.11
 * Simulink Coder version         : 23.2 (R2023b) 01-Aug-2023
 * C/C++ source code generated on : Wed Apr  8 08:52:41 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "mymodel.h"
#include <math.h>
#include "rtwtypes.h"
#include "mymodel_private.h"

/* Block signals (default storage) */
B_mymodel_T mymodel_B;

/* Block states (default storage) */
DW_mymodel_T mymodel_DW;

/* External outputs (root outports fed by signals with default storage) */
ExtY_mymodel_T mymodel_Y;

/* Real-time model */
static RT_MODEL_mymodel_T mymodel_M_;
RT_MODEL_mymodel_T *const mymodel_M = &mymodel_M_;

/* Model step function */

/* Const memory section */
void mymodel_step(void)
{
  /* DiscreteIntegrator: '<Root>/Discrete-Time Integrator1' */
  mymodel_B.x = mymodel_DW.DiscreteTimeIntegrator1_DSTATE;

  /* DiscreteIntegrator: '<Root>/Discrete-Time Integrator' */
  mymodel_B.v = mymodel_DW.DiscreteTimeIntegrator_DSTATE;

  /* Gain: '<Root>/1//Mass ' incorporates:
   *  Gain: '<Root>/Damping'
   *  Gain: '<Root>/Stiffness'
   *  Sin: '<Root>/Sine Wave'
   *  Sum: '<Root>/Add'
   *  Sum: '<Root>/Sum1'
   */
  mymodel_B.a = ((0.0 - ((sin(((real_T)mymodel_DW.counter +
    mymodel_P.SineWave_Offset) * 2.0 * 3.1415926535897931 /
    mymodel_P.SineWave_NumSamp) * mymodel_P.SineWave_Amp +
    mymodel_P.SineWave_Bias) + mymodel_P.k * mymodel_B.x)) - mymodel_P.b *
                 mymodel_B.v) * (1.0 / mymodel_P.m);

  /* Outport: '<Root>/Out1' */
  mymodel_Y.Out1[0] = mymodel_B.x;
  mymodel_Y.Out1[1] = mymodel_B.v;
  mymodel_Y.Out1[2] = mymodel_B.a;

  /* Update for DiscreteIntegrator: '<Root>/Discrete-Time Integrator1' */
  mymodel_DW.DiscreteTimeIntegrator1_DSTATE +=
    mymodel_P.DiscreteTimeIntegrator1_gainval * mymodel_B.v;

  /* Update for DiscreteIntegrator: '<Root>/Discrete-Time Integrator' */
  mymodel_DW.DiscreteTimeIntegrator_DSTATE +=
    mymodel_P.DiscreteTimeIntegrator_gainval * mymodel_B.a;

  /* Update for Sin: '<Root>/Sine Wave' */
  mymodel_DW.counter++;
  if (mymodel_DW.counter == mymodel_P.SineWave_NumSamp) {
    mymodel_DW.counter = 0;
  }

  /* End of Update for Sin: '<Root>/Sine Wave' */
  {                                    /* Sample time: [0.01s, 0.0s] */
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   */
  mymodel_M->Timing.taskTime0 =
    ((time_T)(++mymodel_M->Timing.clockTick0)) * mymodel_M->Timing.stepSize0;
}

/* Model initialize function */

/* Volatile memory section */
void mymodel_initialize(void)
{
  /* Registration code */
  rtmSetTFinal(mymodel_M, -1);
  mymodel_M->Timing.stepSize0 = 0.01;

  /* External mode info */
  mymodel_M->Sizes.checksums[0] = (1822297175U);
  mymodel_M->Sizes.checksums[1] = (771875560U);
  mymodel_M->Sizes.checksums[2] = (1181160525U);
  mymodel_M->Sizes.checksums[3] = (4289910520U);

  {
    static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE;
    static RTWExtModeInfo rt_ExtModeInfo;
    static const sysRanDType *systemRan[1];
    mymodel_M->extModeInfo = (&rt_ExtModeInfo);
    rteiSetSubSystemActiveVectorAddresses(&rt_ExtModeInfo, systemRan);
    systemRan[0] = &rtAlwaysEnabled;
    rteiSetModelMappingInfoPtr(mymodel_M->extModeInfo,
      &mymodel_M->SpecialInfo.mappingInfo);
    rteiSetChecksumsPtr(mymodel_M->extModeInfo, mymodel_M->Sizes.checksums);
    rteiSetTPtr(mymodel_M->extModeInfo, rtmGetTPtr(mymodel_M));
  }

  /* InitializeConditions for DiscreteIntegrator: '<Root>/Discrete-Time Integrator1' */
  mymodel_DW.DiscreteTimeIntegrator1_DSTATE =
    mymodel_P.DiscreteTimeIntegrator1_IC;

  /* InitializeConditions for DiscreteIntegrator: '<Root>/Discrete-Time Integrator' */
  mymodel_DW.DiscreteTimeIntegrator_DSTATE = mymodel_P.DiscreteTimeIntegrator_IC;
}

/* Model terminate function */
void mymodel_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
