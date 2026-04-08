/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: mymodel_data.c
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

/* Block parameters (default storage) */
P_mymodel_T mymodel_P = {
  /* Variable: b
   * Referenced by: '<Root>/Damping'
   */
  10.0,

  /* Variable: k
   * Referenced by: '<Root>/Stiffness'
   */
  400.0,

  /* Variable: m
   * Referenced by: '<Root>/1//Mass '
   */
  3.6,

  /* Expression: 0
   * Referenced by: '<Root>/Data Store Memory'
   */
  0.0,

  /* Computed Parameter: DiscreteTimeIntegrator1_gainval
   * Referenced by: '<Root>/Discrete-Time Integrator1'
   */
  0.01,

  /* Expression: 0
   * Referenced by: '<Root>/Discrete-Time Integrator1'
   */
  0.0,

  /* Computed Parameter: DiscreteTimeIntegrator_gainval
   * Referenced by: '<Root>/Discrete-Time Integrator'
   */
  0.01,

  /* Expression: 0
   * Referenced by: '<Root>/Discrete-Time Integrator'
   */
  0.0,

  /* Expression: 10
   * Referenced by: '<Root>/Sine Wave'
   */
  10.0,

  /* Expression: 0
   * Referenced by: '<Root>/Sine Wave'
   */
  0.0,

  /* Expression: 10
   * Referenced by: '<Root>/Sine Wave'
   */
  10.0,

  /* Expression: 0
   * Referenced by: '<Root>/Sine Wave'
   */
  0.0
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
