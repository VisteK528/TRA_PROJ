/**
  ******************************************************************************
  * @file    speech_network_data_params.h
  * @author  AST Embedded Analytics Research Platform
  * @date    2024-12-29T13:04:26+0100
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#ifndef SPEECH_NETWORK_DATA_PARAMS_H
#define SPEECH_NETWORK_DATA_PARAMS_H

#include "ai_platform.h"

/*
#define AI_SPEECH_NETWORK_DATA_WEIGHTS_PARAMS \
  (AI_HANDLE_PTR(&ai_speech_network_data_weights_params[1]))
*/

#define AI_SPEECH_NETWORK_DATA_CONFIG               (NULL)


#define AI_SPEECH_NETWORK_DATA_ACTIVATIONS_SIZES \
  { 127616, }
#define AI_SPEECH_NETWORK_DATA_ACTIVATIONS_SIZE     (127616)
#define AI_SPEECH_NETWORK_DATA_ACTIVATIONS_COUNT    (1)
#define AI_SPEECH_NETWORK_DATA_ACTIVATION_1_SIZE    (127616)



#define AI_SPEECH_NETWORK_DATA_WEIGHTS_SIZES \
  { 272992, }
#define AI_SPEECH_NETWORK_DATA_WEIGHTS_SIZE         (272992)
#define AI_SPEECH_NETWORK_DATA_WEIGHTS_COUNT        (1)
#define AI_SPEECH_NETWORK_DATA_WEIGHT_1_SIZE        (272992)



#define AI_SPEECH_NETWORK_DATA_ACTIVATIONS_TABLE_GET() \
  (&g_speech_network_activations_table[1])

extern ai_handle g_speech_network_activations_table[1 + 2];



#define AI_SPEECH_NETWORK_DATA_WEIGHTS_TABLE_GET() \
  (&g_speech_network_weights_table[1])

extern ai_handle g_speech_network_weights_table[1 + 2];


#endif    /* SPEECH_NETWORK_DATA_PARAMS_H */
