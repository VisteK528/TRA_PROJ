/**
  ******************************************************************************
  * @file    word_classifier_data_params.h
  * @author  AST Embedded Analytics Research Platform
  * @date    2025-01-19T01:55:29+0100
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#ifndef WORD_CLASSIFIER_DATA_PARAMS_H
#define WORD_CLASSIFIER_DATA_PARAMS_H

#include "ai_platform.h"

/*
#define AI_WORD_CLASSIFIER_DATA_WEIGHTS_PARAMS \
  (AI_HANDLE_PTR(&ai_word_classifier_data_weights_params[1]))
*/

#define AI_WORD_CLASSIFIER_DATA_CONFIG               (NULL)


#define AI_WORD_CLASSIFIER_DATA_ACTIVATIONS_SIZES \
  { 22784, }
#define AI_WORD_CLASSIFIER_DATA_ACTIVATIONS_SIZE     (22784)
#define AI_WORD_CLASSIFIER_DATA_ACTIVATIONS_COUNT    (1)
#define AI_WORD_CLASSIFIER_DATA_ACTIVATION_1_SIZE    (22784)



#define AI_WORD_CLASSIFIER_DATA_WEIGHTS_SIZES \
  { 138528, }
#define AI_WORD_CLASSIFIER_DATA_WEIGHTS_SIZE         (138528)
#define AI_WORD_CLASSIFIER_DATA_WEIGHTS_COUNT        (1)
#define AI_WORD_CLASSIFIER_DATA_WEIGHT_1_SIZE        (138528)



#define AI_WORD_CLASSIFIER_DATA_ACTIVATIONS_TABLE_GET() \
  (&g_word_classifier_activations_table[1])

extern ai_handle g_word_classifier_activations_table[1 + 2];



#define AI_WORD_CLASSIFIER_DATA_WEIGHTS_TABLE_GET() \
  (&g_word_classifier_weights_table[1])

extern ai_handle g_word_classifier_weights_table[1 + 2];


#endif    /* WORD_CLASSIFIER_DATA_PARAMS_H */
