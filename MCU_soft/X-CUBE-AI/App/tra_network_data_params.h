/**
  ******************************************************************************
  * @file    tra_network_data_params.h
  * @author  AST Embedded Analytics Research Platform
  * @date    2025-01-12T16:57:47+0100
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

#ifndef TRA_NETWORK_DATA_PARAMS_H
#define TRA_NETWORK_DATA_PARAMS_H

#include "ai_platform.h"

/*
#define AI_TRA_NETWORK_DATA_WEIGHTS_PARAMS \
  (AI_HANDLE_PTR(&ai_tra_network_data_weights_params[1]))
*/

#define AI_TRA_NETWORK_DATA_CONFIG               (NULL)


#define AI_TRA_NETWORK_DATA_ACTIVATIONS_SIZES \
  { 41636, }
#define AI_TRA_NETWORK_DATA_ACTIVATIONS_SIZE     (41636)
#define AI_TRA_NETWORK_DATA_ACTIVATIONS_COUNT    (1)
#define AI_TRA_NETWORK_DATA_ACTIVATION_1_SIZE    (41636)



#define AI_TRA_NETWORK_DATA_WEIGHTS_SIZES \
  { 100960, }
#define AI_TRA_NETWORK_DATA_WEIGHTS_SIZE         (100960)
#define AI_TRA_NETWORK_DATA_WEIGHTS_COUNT        (1)
#define AI_TRA_NETWORK_DATA_WEIGHT_1_SIZE        (100960)



#define AI_TRA_NETWORK_DATA_ACTIVATIONS_TABLE_GET() \
  (&g_tra_network_activations_table[1])

extern ai_handle g_tra_network_activations_table[1 + 2];



#define AI_TRA_NETWORK_DATA_WEIGHTS_TABLE_GET() \
  (&g_tra_network_weights_table[1])

extern ai_handle g_tra_network_weights_table[1 + 2];


#endif    /* TRA_NETWORK_DATA_PARAMS_H */
