/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: ert_main.c
 *
 * Code generated for Simulink model 'mymodel'.
 *
 * Model version                  : 1.6
 * Simulink Coder version         : 23.2 (R2023b) 01-Aug-2023
 * C/C++ source code generated on : Thu Apr  2 14:12:26 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include <stdio.h>            /* This example main program uses printf/fflush */
#include "mymodel.h"                   /* Model header file */
#include "ext_mode.h"                  /* External mode header file */

/*
 * Associating rt_OneStep with a real-time clock or interrupt service routine
 * is what makes the generated code "real-time".  The function rt_OneStep is
 * always associated with the base rate of the model.  Subrates are managed
 * by the base rate from inside the generated code.  Enabling/disabling
 * interrupts and floating point context switches are target specific.  This
 * example code indicates where these should take place relative to executing
 * the generated code step function.  Overrun behavior should be tailored to
 * your application needs.  This example simply sets an error status in the
 * real-time model and returns from rt_OneStep.
 */
void rt_OneStep(void);
void rt_OneStep(void)
{
  extmodeErrorCode_T errorCode = EXTMODE_SUCCESS;
  extmodeSimulationTime_T currentTime = (extmodeSimulationTime_T) 0;
  static boolean_T OverrunFlag = false;

  /* Disable interrupts here */

  /* Check for overrun */
  if (OverrunFlag) {
    rtmSetErrorStatus(mymodel_M, "Overrun");
    return;
  }

  OverrunFlag = true;

  /* Save FPU context here (if necessary) */
  /* Re-enable timer or interrupt here */
  /* Set model inputs here */
  currentTime = (extmodeSimulationTime_T) mymodel_M->Timing.taskTime0;

  /* Step the model */
  mymodel_step();

  /* Get model outputs here */

  /* Trigger External Mode event */
  errorCode = extmodeEvent(0,currentTime);
  if (errorCode != EXTMODE_SUCCESS) {
    /* Code to handle External Mode event errors
       may be added here */
  }

  /* Indicate task complete */
  OverrunFlag = false;

  /* Disable interrupts here */
  /* Restore FPU context here (if necessary) */
  /* Enable interrupts here */
}

/*
 * The example main function illustrates what is required by your
 * application code to initialize, execute, and terminate the generated code.
 * Attaching rt_OneStep to a real-time clock is target specific. This example
 * illustrates how you do this relative to initializing the model.
 */
int_T main(int_T argc, const char *argv[])
{
  extmodeErrorCode_T errorCode = EXTMODE_SUCCESS;

  /* Parse External Mode command line arguments */
  errorCode = extmodeParseArgs(argc, argv);
  if (errorCode != EXTMODE_SUCCESS) {
    return (errorCode);
  }

  /* Initialize model */
  mymodel_initialize();

  /* External Mode initialization */
  errorCode = extmodeInit(mymodel_M->extModeInfo, &rtmGetTFinal(mymodel_M));
  if (errorCode != EXTMODE_SUCCESS) {
    /* Code to handle External Mode initialization errors
       may be added here */
  }

  if (errorCode == EXTMODE_SUCCESS) {
    /* Wait until a Start or Stop Request has been received from the Host */
    extmodeWaitForHostRequest(EXTMODE_WAIT_FOREVER);
    if (extmodeStopRequested()) {
      rtmSetStopRequested(mymodel_M, true);
    }
  }

  /* The External Mode option selected; therefore,
   *  simulating the model step behavior (in non real-time).
   */
  while (!extmodeSimulationComplete() && !extmodeStopRequested() &&
         !rtmGetStopRequested(mymodel_M)) {
    /* Run External Mode background activities */
    errorCode = extmodeBackgroundRun();
    if (errorCode != EXTMODE_SUCCESS) {
      /* Code to handle External Mode background task errors
         may be added here */
    }

    rt_OneStep();
  }

  /* Terminate model */
  mymodel_terminate();

  /* External Mode reset */
  extmodeReset();
  return 0;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
