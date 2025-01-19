/**
  ******************************************************************************
  * @file    word_classifier.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2025-01-19T01:55:29+0100
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */


#include "word_classifier.h"
#include "word_classifier_data.h"

#include "ai_platform.h"
#include "ai_platform_interface.h"
#include "ai_math_helpers.h"

#include "core_common.h"
#include "core_convert.h"

#include "layers.h"



#undef AI_NET_OBJ_INSTANCE
#define AI_NET_OBJ_INSTANCE g_word_classifier
 
#undef AI_WORD_CLASSIFIER_MODEL_SIGNATURE
#define AI_WORD_CLASSIFIER_MODEL_SIGNATURE     "0x73e8354ecfe0ee74f0a2f4c71cfc8e7b"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     ""
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "2025-01-19T01:55:29+0100"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_WORD_CLASSIFIER_N_BATCHES
#define AI_WORD_CLASSIFIER_N_BATCHES         (1)

static ai_ptr g_word_classifier_activations_map[1] = AI_C_ARRAY_INIT;
static ai_ptr g_word_classifier_weights_map[1] = AI_C_ARRAY_INIT;



/**  Array declarations section  **********************************************/
/* Array#0 */
AI_ARRAY_OBJ_DECLARE(
  serving_default_input_50_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 1024, AI_STATIC)

/* Array#1 */
AI_ARRAY_OBJ_DECLARE(
  conversion_0_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 1024, AI_STATIC)

/* Array#2 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_1_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 14400, AI_STATIC)

/* Array#3 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_2_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 6272, AI_STATIC)

/* Array#4 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_4_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 4608, AI_STATIC)

/* Array#5 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_5_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 6400, AI_STATIC)

/* Array#6 */
AI_ARRAY_OBJ_DECLARE(
  gemm_7_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 16, AI_STATIC)

/* Array#7 */
AI_ARRAY_OBJ_DECLARE(
  gemm_8_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 128, AI_STATIC)

/* Array#8 */
AI_ARRAY_OBJ_DECLARE(
  gemm_9_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 4, AI_STATIC)

/* Array#9 */
AI_ARRAY_OBJ_DECLARE(
  nl_10_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 4, AI_STATIC)

/* Array#10 */
AI_ARRAY_OBJ_DECLARE(
  conversion_11_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 4, AI_STATIC)

/* Array#11 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_1_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 144, AI_STATIC)

/* Array#12 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_1_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 16, AI_STATIC)

/* Array#13 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_2_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 4608, AI_STATIC)

/* Array#14 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_2_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 32, AI_STATIC)

/* Array#15 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_4_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 9216, AI_STATIC)

/* Array#16 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_4_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 32, AI_STATIC)

/* Array#17 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_5_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 18432, AI_STATIC)

/* Array#18 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_5_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 64, AI_STATIC)

/* Array#19 */
AI_ARRAY_OBJ_DECLARE(
  gemm_7_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 102400, AI_STATIC)

/* Array#20 */
AI_ARRAY_OBJ_DECLARE(
  gemm_7_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 16, AI_STATIC)

/* Array#21 */
AI_ARRAY_OBJ_DECLARE(
  gemm_8_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 2048, AI_STATIC)

/* Array#22 */
AI_ARRAY_OBJ_DECLARE(
  gemm_8_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 128, AI_STATIC)

/* Array#23 */
AI_ARRAY_OBJ_DECLARE(
  gemm_9_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 512, AI_STATIC)

/* Array#24 */
AI_ARRAY_OBJ_DECLARE(
  gemm_9_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 4, AI_STATIC)

/* Array#25 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_1_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 548, AI_STATIC)

/* Array#26 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_2_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 6144, AI_STATIC)

/* Array#27 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_2_scratch1_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 1792, AI_STATIC)

/* Array#28 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_4_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 6720, AI_STATIC)

/* Array#29 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_5_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 7168, AI_STATIC)

/* Array#30 */
AI_ARRAY_OBJ_DECLARE(
  gemm_7_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 6480, AI_STATIC)

/* Array#31 */
AI_ARRAY_OBJ_DECLARE(
  gemm_8_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 656, AI_STATIC)

/* Array#32 */
AI_ARRAY_OBJ_DECLARE(
  gemm_9_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 148, AI_STATIC)

/* Array#33 */
AI_ARRAY_OBJ_DECLARE(
  nl_10_scratch0_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 1, AI_STATIC)

/**  Array metadata declarations section  *************************************/
/* Int quant #0 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conversion_0_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(118.0536117553711f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #1 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_1_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(64.04903411865234f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #2 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_2_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(93.70098876953125f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #3 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_4_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(145.46844482421875f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #4 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_5_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(102.85371398925781f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #5 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_7_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(183.6317901611328f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #6 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_8_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(88.4465103149414f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #7 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_9_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(294.45379638671875f),
    AI_PACK_INTQ_ZP(-12)))

/* Int quant #8 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(nl_10_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.00390625f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #9 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_1_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 16,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.001988318283110857f, 0.001123588066548109f, 0.0019371163798496127f, 0.001929433667100966f, 0.0019192270701751113f, 0.001801675301976502f, 0.0016273154178634286f, 0.002164730103686452f, 0.0018706141272559762f, 0.00187613011803478f, 0.0020057379733771086f, 0.0019041409250348806f, 0.001939401845447719f, 0.002021356485784054f, 0.0017763637006282806f, 0.001196294673718512f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #10 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_2_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 32,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.001335797249339521f, 0.0013249203329905868f, 0.0016007196391001344f, 0.0012829946354031563f, 0.0016290538478642702f, 0.0015611648559570312f, 0.0013773570535704494f, 0.0012791401240974665f, 0.0012603740906342864f, 0.001250941539183259f, 0.0016243037534877658f, 0.0016133141471073031f, 0.0012008837657049298f, 0.0016283493023365736f, 0.0015844962326809764f, 0.0013485255185514688f, 0.0018678036285564303f, 0.0012868148041889071f, 0.0013582701794803143f, 0.0013491595163941383f, 0.0014301418559625745f, 0.0013425123179331422f, 0.0014874555636197329f, 0.0012320680543780327f, 0.001243367325514555f, 0.001777303172275424f, 0.0012596636079251766f, 0.0013008264359086752f, 0.0012312486069276929f, 0.0012447801418602467f, 0.0015691487351432443f, 0.0012327589793130755f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #11 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_4_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 32,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0014575981767848134f, 0.0015284748515114188f, 0.0013170206220820546f, 0.00101719971280545f, 0.0013562985695898533f, 0.001347587676718831f, 0.0013730587670579553f, 0.0010468156542629004f, 0.0013092992594465613f, 0.0013513704761862755f, 0.0012765397550538182f, 0.0011673405533656478f, 0.0014015320921316743f, 0.0010639664251357317f, 0.0013891394482925534f, 0.0012064147740602493f, 0.0011839654762297869f, 0.0015841919230297208f, 0.001235986826941371f, 0.0012707929126918316f, 0.001433591009117663f, 0.0013813445111736655f, 0.0011918789241462946f, 0.0014221161836758256f, 0.0012580821057781577f, 0.001363971852697432f, 0.001443939283490181f, 0.0015920583391562104f, 0.0012155293952673674f, 0.0013316302793100476f, 0.0013320940779522061f, 0.0011894123163074255f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #12 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_5_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 64,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0012607583776116371f, 0.0013097672490403056f, 0.0012226431863382459f, 0.0013273805379867554f, 0.0011488914024084806f, 0.001003884943202138f, 0.001421644352376461f, 0.001005446189083159f, 0.001135039608925581f, 0.0010031944839283824f, 0.0011270847171545029f, 0.0009637554176151752f, 0.0011479343520477414f, 0.001042171847075224f, 0.0010451368289068341f, 0.0009565785876475275f, 0.0010137991048395634f, 0.001064496231265366f, 0.0010462342761456966f, 0.0009305297862738371f, 0.0010750711662694812f, 0.0008816357585601509f, 0.0012247650884091854f, 0.0009836623212322593f, 0.0009585447260178626f, 0.0008506915764883161f, 0.0011063828133046627f, 0.0008257223526015878f, 0.0010108554270118475f, 0.0011123325675725937f, 0.0012353940401226282f, 0.001053332700394094f, 0.0009629674605093896f, 0.0010518237249925733f, 0.0011366762919351459f, 0.0010578541550785303f, 0.0009250336443074048f, 0.0009842501021921635f, 0.001104112365283072f, 0.0010153563925996423f, 0.0009213140001520514f, 0.0009654554887674749f, 0.0011536419624462724f, 0.0011245835339650512f, 0.0009634363814257085f, 0.0010066760005429387f, 0.0011090844636783004f, 0.0009622426587156951f, 0.00088907202007249f, 0.0011342719662934542f, 0.0012271805899217725f, 0.0011593047529459f, 0.0011724128853529692f, 0.0012754126219078898f, 0.0010222871787846088f, 0.000991813256405294f, 0.0008746552630327642f, 0.001272788387723267f, 0.0011719476897269487f, 0.001002715085633099f, 0.0009722030954435468f, 0.0009355738293379545f, 0.0010655669029802084f, 0.0012142328778281808f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #13 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_7_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 16,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.00028680753894150257f, 0.0009916058043017983f, 0.0010040609631687403f, 0.0009712442406453192f, 0.0008558132103644311f, 0.0008927288581617177f, 0.0009388518519699574f, 0.0007919779745861888f, 0.0007384600467048585f, 0.0008903456036932766f, 0.0006908854120410979f, 0.000848804076667875f, 0.0008623625035397708f, 0.0010109671857208014f, 0.0011015902273356915f, 0.0008636947604827583f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #14 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_8_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 128,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0015016768593341112f, 0.0017424222314730287f, 0.001994286896660924f, 0.0016349947545677423f, 0.0013022713828831911f, 0.0014216955751180649f, 0.0015362798003479838f, 0.0014898204244673252f, 0.0016940165078267455f, 0.0020404013339430094f, 0.001886840327642858f, 0.0014256850117817521f, 0.0013670395128428936f, 0.0016823727637529373f, 0.0018023396842181683f, 0.001998394727706909f, 0.0018366617150604725f, 0.002009505871683359f, 0.0017978058895096183f, 0.0018529457738623023f, 0.0018362830160185695f, 0.0018157347803935409f, 0.0018175450386479497f, 0.0016512545989826322f, 0.0013769245706498623f, 0.0020809618290513754f, 0.0020576142705976963f, 0.0016935310559347272f, 0.0017446846468374133f, 0.0018450720235705376f, 0.0018568967934697866f, 0.0016300973948091269f, 0.0017012548632919788f, 0.001545243663713336f, 0.0017887093126773834f, 0.0014650451485067606f, 0.0018721085507422686f, 0.0016692643985152245f, 0.0018911062506958842f, 0.001767096808180213f, 0.0016733233351260424f, 0.0021926448680460453f, 0.0017610049108043313f, 0.0017255753045901656f, 0.0017855321057140827f, 0.0016461231280118227f, 0.001619945396669209f, 0.0014528456376865506f, 0.0015523986658081412f, 0.0020853097084909678f, 0.0017580199055373669f, 0.0018892696825787425f, 0.0021107203792780638f, 0.0017769229598343372f, 0.0019474494038149714f, 0.0016589909791946411f, 0.0016471921699121594f, 0.0018455020617693663f, 0.0017750096740201116f, 0.00117961666546762f, 0.001852944609709084f, 0.0016806789208203554f, 0.0018803456332534552f, 0.0018495521508157253f, 0.0017365756211802363f, 0.0018963550683110952f, 0.001513263676315546f, 0.0014012159081175923f, 0.0016981508815661073f, 0.0015723517863079906f, 0.0017622042214497924f, 0.0015673612942919135f, 0.0018925168551504612f, 0.0017079929821193218f, 0.001955699874088168f, 0.0017835281323641539f, 0.0015481894370168447f, 0.0018598265014588833f, 0.0018563823541626334f, 0.0016281805001199245f, 0.0015137644950300455f, 0.0018319623777642846f, 0.002068969653919339f, 0.0017283579800277948f, 0.001619542483240366f, 0.0018175676232203841f, 0.0018404779257252812f, 0.0020166439935564995f, 0.001732157077640295f, 0.0017696110298857093f, 0.001825489685870707f, 0.0018739098450168967f, 0.0019299917621538043f, 0.0016412633704021573f, 0.0019035887671634555f, 0.0015773302875459194f, 0.0012057287385687232f, 0.0018558187875896692f, 0.0017308710375800729f, 0.0013117191847413778f, 0.001930678146891296f, 0.0016463849460706115f, 0.0016189741436392069f, 0.0015656979521736503f, 0.0018883245065808296f, 0.0019115550676360726f, 0.001366970012895763f, 0.0014847870916128159f, 0.0016655490035191178f, 0.0019956419710069895f, 0.0016744796885177493f, 0.0016727828187868f, 0.0021705664694309235f, 0.0016968714771792293f, 0.0018209663685411215f, 0.001669246586970985f, 0.0016438080929219723f, 0.0018951701931655407f, 0.0016307678306475282f, 0.001743494882248342f, 0.00174154550768435f, 0.0015878832200542092f, 0.0018762092804536223f, 0.0016902260249480605f, 0.001526312786154449f, 0.0018441244028508663f, 0.0014432674506679177f, 0.0015727238496765494f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #15 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_9_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 4,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.002058702986687422f, 0.0023035446647554636f, 0.0023035071790218353f, 0.002283677691593766f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0)))

/* Int quant #16 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_2_scratch1_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(93.70098876953125f),
    AI_PACK_INTQ_ZP(-128)))

/**  Tensor declarations section  *********************************************/
/* Tensor #0 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_5_scratch0, AI_STATIC,
  0, 0x0,
  AI_SHAPE_INIT(4, 1, 7168, 1, 1), AI_STRIDE_INIT(4, 1, 1, 7168, 7168),
  1, &conv2d_5_scratch0_array, NULL)

/* Tensor #1 */
AI_TENSOR_OBJ_DECLARE(
  gemm_7_scratch0, AI_STATIC,
  1, 0x0,
  AI_SHAPE_INIT(4, 1, 6480, 1, 1), AI_STRIDE_INIT(4, 2, 2, 12960, 12960),
  1, &gemm_7_scratch0_array, NULL)

/* Tensor #2 */
AI_TENSOR_OBJ_DECLARE(
  gemm_8_scratch0, AI_STATIC,
  2, 0x0,
  AI_SHAPE_INIT(4, 1, 656, 1, 1), AI_STRIDE_INIT(4, 2, 2, 1312, 1312),
  1, &gemm_8_scratch0_array, NULL)

/* Tensor #3 */
AI_TENSOR_OBJ_DECLARE(
  gemm_9_scratch0, AI_STATIC,
  3, 0x0,
  AI_SHAPE_INIT(4, 1, 148, 1, 1), AI_STRIDE_INIT(4, 2, 2, 296, 296),
  1, &gemm_9_scratch0_array, NULL)

/* Tensor #4 */
AI_TENSOR_OBJ_DECLARE(
  nl_10_scratch0, AI_STATIC,
  4, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &nl_10_scratch0_array, NULL)

/* Tensor #5 */
AI_TENSOR_OBJ_DECLARE(
  serving_default_input_50_output, AI_STATIC,
  5, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 32, 32), AI_STRIDE_INIT(4, 4, 4, 4, 128),
  1, &serving_default_input_50_output_array, NULL)

/* Tensor #6 */
AI_TENSOR_OBJ_DECLARE(
  conversion_0_output, AI_STATIC,
  6, 0x1,
  AI_SHAPE_INIT(4, 1, 1, 32, 32), AI_STRIDE_INIT(4, 1, 1, 1, 32),
  1, &conversion_0_output_array, &conversion_0_output_array_intq)

/* Tensor #7 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_1_output, AI_STATIC,
  7, 0x1,
  AI_SHAPE_INIT(4, 1, 16, 30, 30), AI_STRIDE_INIT(4, 1, 1, 16, 480),
  1, &conv2d_1_output_array, &conv2d_1_output_array_intq)

/* Tensor #8 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_2_output, AI_STATIC,
  8, 0x1,
  AI_SHAPE_INIT(4, 1, 32, 14, 14), AI_STRIDE_INIT(4, 1, 1, 32, 448),
  1, &conv2d_2_output_array, &conv2d_2_output_array_intq)

/* Tensor #9 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_4_output, AI_STATIC,
  9, 0x1,
  AI_SHAPE_INIT(4, 1, 32, 12, 12), AI_STRIDE_INIT(4, 1, 1, 32, 384),
  1, &conv2d_4_output_array, &conv2d_4_output_array_intq)

/* Tensor #10 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_5_output, AI_STATIC,
  10, 0x1,
  AI_SHAPE_INIT(4, 1, 64, 10, 10), AI_STRIDE_INIT(4, 1, 1, 64, 640),
  1, &conv2d_5_output_array, &conv2d_5_output_array_intq)

/* Tensor #11 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_5_output0, AI_STATIC,
  11, 0x1,
  AI_SHAPE_INIT(4, 1, 6400, 1, 1), AI_STRIDE_INIT(4, 1, 1, 6400, 6400),
  1, &conv2d_5_output_array, &conv2d_5_output_array_intq)

/* Tensor #12 */
AI_TENSOR_OBJ_DECLARE(
  gemm_7_output, AI_STATIC,
  12, 0x1,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 1, 1, 16, 16),
  1, &gemm_7_output_array, &gemm_7_output_array_intq)

/* Tensor #13 */
AI_TENSOR_OBJ_DECLARE(
  gemm_8_output, AI_STATIC,
  13, 0x1,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 1, 1, 128, 128),
  1, &gemm_8_output_array, &gemm_8_output_array_intq)

/* Tensor #14 */
AI_TENSOR_OBJ_DECLARE(
  gemm_9_output, AI_STATIC,
  14, 0x1,
  AI_SHAPE_INIT(4, 1, 4, 1, 1), AI_STRIDE_INIT(4, 1, 1, 4, 4),
  1, &gemm_9_output_array, &gemm_9_output_array_intq)

/* Tensor #15 */
AI_TENSOR_OBJ_DECLARE(
  nl_10_output, AI_STATIC,
  15, 0x1,
  AI_SHAPE_INIT(4, 1, 4, 1, 1), AI_STRIDE_INIT(4, 1, 1, 4, 4),
  1, &nl_10_output_array, &nl_10_output_array_intq)

/* Tensor #16 */
AI_TENSOR_OBJ_DECLARE(
  conversion_11_output, AI_STATIC,
  16, 0x0,
  AI_SHAPE_INIT(4, 1, 4, 1, 1), AI_STRIDE_INIT(4, 4, 4, 16, 16),
  1, &conversion_11_output_array, NULL)

/* Tensor #17 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_1_weights, AI_STATIC,
  17, 0x1,
  AI_SHAPE_INIT(4, 1, 3, 3, 16), AI_STRIDE_INIT(4, 1, 1, 16, 48),
  1, &conv2d_1_weights_array, &conv2d_1_weights_array_intq)

/* Tensor #18 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_1_bias, AI_STATIC,
  18, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &conv2d_1_bias_array, NULL)

/* Tensor #19 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_2_weights, AI_STATIC,
  19, 0x1,
  AI_SHAPE_INIT(4, 16, 3, 3, 32), AI_STRIDE_INIT(4, 1, 16, 512, 1536),
  1, &conv2d_2_weights_array, &conv2d_2_weights_array_intq)

/* Tensor #20 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_2_bias, AI_STATIC,
  20, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &conv2d_2_bias_array, NULL)

/* Tensor #21 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_4_weights, AI_STATIC,
  21, 0x1,
  AI_SHAPE_INIT(4, 32, 3, 3, 32), AI_STRIDE_INIT(4, 1, 32, 1024, 3072),
  1, &conv2d_4_weights_array, &conv2d_4_weights_array_intq)

/* Tensor #22 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_4_bias, AI_STATIC,
  22, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &conv2d_4_bias_array, NULL)

/* Tensor #23 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_5_weights, AI_STATIC,
  23, 0x1,
  AI_SHAPE_INIT(4, 32, 3, 3, 64), AI_STRIDE_INIT(4, 1, 32, 2048, 6144),
  1, &conv2d_5_weights_array, &conv2d_5_weights_array_intq)

/* Tensor #24 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_5_bias, AI_STATIC,
  24, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &conv2d_5_bias_array, NULL)

/* Tensor #25 */
AI_TENSOR_OBJ_DECLARE(
  gemm_7_weights, AI_STATIC,
  25, 0x1,
  AI_SHAPE_INIT(4, 6400, 16, 1, 1), AI_STRIDE_INIT(4, 1, 6400, 102400, 102400),
  1, &gemm_7_weights_array, &gemm_7_weights_array_intq)

/* Tensor #26 */
AI_TENSOR_OBJ_DECLARE(
  gemm_7_bias, AI_STATIC,
  26, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &gemm_7_bias_array, NULL)

/* Tensor #27 */
AI_TENSOR_OBJ_DECLARE(
  gemm_8_weights, AI_STATIC,
  27, 0x1,
  AI_SHAPE_INIT(4, 16, 128, 1, 1), AI_STRIDE_INIT(4, 1, 16, 2048, 2048),
  1, &gemm_8_weights_array, &gemm_8_weights_array_intq)

/* Tensor #28 */
AI_TENSOR_OBJ_DECLARE(
  gemm_8_bias, AI_STATIC,
  28, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &gemm_8_bias_array, NULL)

/* Tensor #29 */
AI_TENSOR_OBJ_DECLARE(
  gemm_9_weights, AI_STATIC,
  29, 0x1,
  AI_SHAPE_INIT(4, 128, 4, 1, 1), AI_STRIDE_INIT(4, 1, 128, 512, 512),
  1, &gemm_9_weights_array, &gemm_9_weights_array_intq)

/* Tensor #30 */
AI_TENSOR_OBJ_DECLARE(
  gemm_9_bias, AI_STATIC,
  30, 0x0,
  AI_SHAPE_INIT(4, 1, 4, 1, 1), AI_STRIDE_INIT(4, 4, 4, 16, 16),
  1, &gemm_9_bias_array, NULL)

/* Tensor #31 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_1_scratch0, AI_STATIC,
  31, 0x0,
  AI_SHAPE_INIT(4, 1, 548, 1, 1), AI_STRIDE_INIT(4, 1, 1, 548, 548),
  1, &conv2d_1_scratch0_array, NULL)

/* Tensor #32 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_2_scratch0, AI_STATIC,
  32, 0x0,
  AI_SHAPE_INIT(4, 1, 6144, 1, 1), AI_STRIDE_INIT(4, 1, 1, 6144, 6144),
  1, &conv2d_2_scratch0_array, NULL)

/* Tensor #33 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_2_scratch1, AI_STATIC,
  33, 0x1,
  AI_SHAPE_INIT(4, 1, 32, 28, 2), AI_STRIDE_INIT(4, 1, 1, 32, 896),
  1, &conv2d_2_scratch1_array, &conv2d_2_scratch1_array_intq)

/* Tensor #34 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_4_scratch0, AI_STATIC,
  34, 0x0,
  AI_SHAPE_INIT(4, 1, 6720, 1, 1), AI_STRIDE_INIT(4, 1, 1, 6720, 6720),
  1, &conv2d_4_scratch0_array, NULL)



/**  Layer declarations section  **********************************************/


AI_TENSOR_CHAIN_OBJ_DECLARE(
  conversion_11_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &nl_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conversion_11_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conversion_11_layer, 11,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conversion_11_chain,
  NULL, &conversion_11_layer, AI_STATIC, 
)


AI_STATIC_CONST ai_i32 nl_10_nl_params_data[] = { 2147483647, 30, -1 };
AI_ARRAY_OBJ_DECLARE(
    nl_10_nl_params, AI_ARRAY_FORMAT_S32,
    nl_10_nl_params_data, nl_10_nl_params_data, 3, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  nl_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_9_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &nl_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &nl_10_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  nl_10_layer, 10,
  NL_TYPE, 0x0, NULL,
  nl, forward_sm_integer,
  &nl_10_chain,
  NULL, &conversion_11_layer, AI_STATIC, 
  .nl_params = &nl_10_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  gemm_9_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_8_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_9_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &gemm_9_weights, &gemm_9_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_9_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  gemm_9_layer, 9,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA_ch,
  &gemm_9_chain,
  NULL, &nl_10_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  gemm_8_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_7_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_8_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &gemm_8_weights, &gemm_8_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_8_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  gemm_8_layer, 8,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA_ch,
  &gemm_8_chain,
  NULL, &gemm_9_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  gemm_7_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_5_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_7_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &gemm_7_weights, &gemm_7_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_7_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  gemm_7_layer, 7,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA_ch,
  &gemm_7_chain,
  NULL, &gemm_8_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_5_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_4_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_5_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv2d_5_weights, &conv2d_5_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_5_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv2d_5_layer, 5,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_deep_3x3_sssa8_ch,
  &conv2d_5_chain,
  NULL, &gemm_7_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_4_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_2_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_4_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv2d_4_weights, &conv2d_4_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_4_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv2d_4_layer, 4,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_deep_3x3_sssa8_ch,
  &conv2d_4_chain,
  NULL, &conv2d_5_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_2_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_2_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv2d_2_weights, &conv2d_2_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv2d_2_scratch0, &conv2d_2_scratch1)
)

AI_LAYER_OBJ_DECLARE(
  conv2d_2_layer, 3,
  OPTIMIZED_CONV2D_TYPE, 0x0, NULL,
  conv2d_nl_pool,  forward_conv2d_deep_3x3_sssa8_ch_nl_pool,
  &conv2d_2_chain,
  NULL, &conv2d_4_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .pool_size = AI_SHAPE_2D_INIT(2, 2), 
  .pool_stride = AI_SHAPE_2D_INIT(2, 2), 
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .pool_func = AI_HANDLE_PTR(pool_func_mp_array_integer_INT8), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conversion_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv2d_1_weights, &conv2d_1_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_1_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv2d_1_layer, 1,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_sssa8_ch,
  &conv2d_1_chain,
  NULL, &conv2d_2_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conversion_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &serving_default_input_50_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conversion_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conversion_0_layer, 0,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conversion_0_chain,
  NULL, &conv2d_1_layer, AI_STATIC, 
)


#if (AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 138528, 1, 1),
    138528, NULL, NULL),
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 22784, 1, 1),
    22784, NULL, NULL),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_WORD_CLASSIFIER_IN_NUM, &serving_default_input_50_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_WORD_CLASSIFIER_OUT_NUM, &conversion_11_output),
  &conversion_0_layer, 0xf09d3d42, NULL)

#else

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 138528, 1, 1),
      138528, NULL, NULL)
  ),
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 22784, 1, 1),
      22784, NULL, NULL)
  ),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_WORD_CLASSIFIER_IN_NUM, &serving_default_input_50_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_WORD_CLASSIFIER_OUT_NUM, &conversion_11_output),
  &conversion_0_layer, 0xf09d3d42, NULL)

#endif	/*(AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)*/



/******************************************************************************/
AI_DECLARE_STATIC
ai_bool word_classifier_configure_activations(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_activations_map(g_word_classifier_activations_map, 1, params)) {
    /* Updating activations (byte) offsets */
    
    serving_default_input_50_output_array.data = AI_PTR(g_word_classifier_activations_map[0] + 11300);
    serving_default_input_50_output_array.data_start = AI_PTR(g_word_classifier_activations_map[0] + 11300);
    conversion_0_output_array.data = AI_PTR(g_word_classifier_activations_map[0] + 15396);
    conversion_0_output_array.data_start = AI_PTR(g_word_classifier_activations_map[0] + 15396);
    conv2d_1_scratch0_array.data = AI_PTR(g_word_classifier_activations_map[0] + 14848);
    conv2d_1_scratch0_array.data_start = AI_PTR(g_word_classifier_activations_map[0] + 14848);
    conv2d_1_output_array.data = AI_PTR(g_word_classifier_activations_map[0] + 448);
    conv2d_1_output_array.data_start = AI_PTR(g_word_classifier_activations_map[0] + 448);
    conv2d_2_scratch0_array.data = AI_PTR(g_word_classifier_activations_map[0] + 14848);
    conv2d_2_scratch0_array.data_start = AI_PTR(g_word_classifier_activations_map[0] + 14848);
    conv2d_2_scratch1_array.data = AI_PTR(g_word_classifier_activations_map[0] + 20992);
    conv2d_2_scratch1_array.data_start = AI_PTR(g_word_classifier_activations_map[0] + 20992);
    conv2d_2_output_array.data = AI_PTR(g_word_classifier_activations_map[0] + 0);
    conv2d_2_output_array.data_start = AI_PTR(g_word_classifier_activations_map[0] + 0);
    conv2d_4_scratch0_array.data = AI_PTR(g_word_classifier_activations_map[0] + 6272);
    conv2d_4_scratch0_array.data_start = AI_PTR(g_word_classifier_activations_map[0] + 6272);
    conv2d_4_output_array.data = AI_PTR(g_word_classifier_activations_map[0] + 18176);
    conv2d_4_output_array.data_start = AI_PTR(g_word_classifier_activations_map[0] + 18176);
    conv2d_5_scratch0_array.data = AI_PTR(g_word_classifier_activations_map[0] + 0);
    conv2d_5_scratch0_array.data_start = AI_PTR(g_word_classifier_activations_map[0] + 0);
    conv2d_5_output_array.data = AI_PTR(g_word_classifier_activations_map[0] + 14976);
    conv2d_5_output_array.data_start = AI_PTR(g_word_classifier_activations_map[0] + 14976);
    gemm_7_scratch0_array.data = AI_PTR(g_word_classifier_activations_map[0] + 0);
    gemm_7_scratch0_array.data_start = AI_PTR(g_word_classifier_activations_map[0] + 0);
    gemm_7_output_array.data = AI_PTR(g_word_classifier_activations_map[0] + 12960);
    gemm_7_output_array.data_start = AI_PTR(g_word_classifier_activations_map[0] + 12960);
    gemm_8_scratch0_array.data = AI_PTR(g_word_classifier_activations_map[0] + 0);
    gemm_8_scratch0_array.data_start = AI_PTR(g_word_classifier_activations_map[0] + 0);
    gemm_8_output_array.data = AI_PTR(g_word_classifier_activations_map[0] + 1312);
    gemm_8_output_array.data_start = AI_PTR(g_word_classifier_activations_map[0] + 1312);
    gemm_9_scratch0_array.data = AI_PTR(g_word_classifier_activations_map[0] + 0);
    gemm_9_scratch0_array.data_start = AI_PTR(g_word_classifier_activations_map[0] + 0);
    gemm_9_output_array.data = AI_PTR(g_word_classifier_activations_map[0] + 296);
    gemm_9_output_array.data_start = AI_PTR(g_word_classifier_activations_map[0] + 296);
    nl_10_scratch0_array.data = AI_PTR(g_word_classifier_activations_map[0] + 0);
    nl_10_scratch0_array.data_start = AI_PTR(g_word_classifier_activations_map[0] + 0);
    nl_10_output_array.data = AI_PTR(g_word_classifier_activations_map[0] + 4);
    nl_10_output_array.data_start = AI_PTR(g_word_classifier_activations_map[0] + 4);
    conversion_11_output_array.data = AI_PTR(g_word_classifier_activations_map[0] + 8);
    conversion_11_output_array.data_start = AI_PTR(g_word_classifier_activations_map[0] + 8);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_ACTIVATIONS);
  return false;
}




/******************************************************************************/
AI_DECLARE_STATIC
ai_bool word_classifier_configure_weights(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_weights_map(g_word_classifier_weights_map, 1, params)) {
    /* Updating weights (byte) offsets */
    
    conv2d_1_weights_array.format |= AI_FMT_FLAG_CONST;
    conv2d_1_weights_array.data = AI_PTR(g_word_classifier_weights_map[0] + 0);
    conv2d_1_weights_array.data_start = AI_PTR(g_word_classifier_weights_map[0] + 0);
    conv2d_1_bias_array.format |= AI_FMT_FLAG_CONST;
    conv2d_1_bias_array.data = AI_PTR(g_word_classifier_weights_map[0] + 144);
    conv2d_1_bias_array.data_start = AI_PTR(g_word_classifier_weights_map[0] + 144);
    conv2d_2_weights_array.format |= AI_FMT_FLAG_CONST;
    conv2d_2_weights_array.data = AI_PTR(g_word_classifier_weights_map[0] + 208);
    conv2d_2_weights_array.data_start = AI_PTR(g_word_classifier_weights_map[0] + 208);
    conv2d_2_bias_array.format |= AI_FMT_FLAG_CONST;
    conv2d_2_bias_array.data = AI_PTR(g_word_classifier_weights_map[0] + 4816);
    conv2d_2_bias_array.data_start = AI_PTR(g_word_classifier_weights_map[0] + 4816);
    conv2d_4_weights_array.format |= AI_FMT_FLAG_CONST;
    conv2d_4_weights_array.data = AI_PTR(g_word_classifier_weights_map[0] + 4944);
    conv2d_4_weights_array.data_start = AI_PTR(g_word_classifier_weights_map[0] + 4944);
    conv2d_4_bias_array.format |= AI_FMT_FLAG_CONST;
    conv2d_4_bias_array.data = AI_PTR(g_word_classifier_weights_map[0] + 14160);
    conv2d_4_bias_array.data_start = AI_PTR(g_word_classifier_weights_map[0] + 14160);
    conv2d_5_weights_array.format |= AI_FMT_FLAG_CONST;
    conv2d_5_weights_array.data = AI_PTR(g_word_classifier_weights_map[0] + 14288);
    conv2d_5_weights_array.data_start = AI_PTR(g_word_classifier_weights_map[0] + 14288);
    conv2d_5_bias_array.format |= AI_FMT_FLAG_CONST;
    conv2d_5_bias_array.data = AI_PTR(g_word_classifier_weights_map[0] + 32720);
    conv2d_5_bias_array.data_start = AI_PTR(g_word_classifier_weights_map[0] + 32720);
    gemm_7_weights_array.format |= AI_FMT_FLAG_CONST;
    gemm_7_weights_array.data = AI_PTR(g_word_classifier_weights_map[0] + 32976);
    gemm_7_weights_array.data_start = AI_PTR(g_word_classifier_weights_map[0] + 32976);
    gemm_7_bias_array.format |= AI_FMT_FLAG_CONST;
    gemm_7_bias_array.data = AI_PTR(g_word_classifier_weights_map[0] + 135376);
    gemm_7_bias_array.data_start = AI_PTR(g_word_classifier_weights_map[0] + 135376);
    gemm_8_weights_array.format |= AI_FMT_FLAG_CONST;
    gemm_8_weights_array.data = AI_PTR(g_word_classifier_weights_map[0] + 135440);
    gemm_8_weights_array.data_start = AI_PTR(g_word_classifier_weights_map[0] + 135440);
    gemm_8_bias_array.format |= AI_FMT_FLAG_CONST;
    gemm_8_bias_array.data = AI_PTR(g_word_classifier_weights_map[0] + 137488);
    gemm_8_bias_array.data_start = AI_PTR(g_word_classifier_weights_map[0] + 137488);
    gemm_9_weights_array.format |= AI_FMT_FLAG_CONST;
    gemm_9_weights_array.data = AI_PTR(g_word_classifier_weights_map[0] + 138000);
    gemm_9_weights_array.data_start = AI_PTR(g_word_classifier_weights_map[0] + 138000);
    gemm_9_bias_array.format |= AI_FMT_FLAG_CONST;
    gemm_9_bias_array.data = AI_PTR(g_word_classifier_weights_map[0] + 138512);
    gemm_9_bias_array.data_start = AI_PTR(g_word_classifier_weights_map[0] + 138512);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_WEIGHTS);
  return false;
}


/**  PUBLIC APIs SECTION  *****************************************************/



AI_DEPRECATED
AI_API_ENTRY
ai_bool ai_word_classifier_get_info(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_WORD_CLASSIFIER_MODEL_NAME,
      .model_signature   = AI_WORD_CLASSIFIER_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 7045032,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .params            = AI_STRUCT_INIT,
      .activations       = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0xf09d3d42,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}



AI_API_ENTRY
ai_bool ai_word_classifier_get_report(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_WORD_CLASSIFIER_MODEL_NAME,
      .model_signature   = AI_WORD_CLASSIFIER_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 7045032,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .map_signature     = AI_MAGIC_SIGNATURE,
      .map_weights       = AI_STRUCT_INIT,
      .map_activations   = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0xf09d3d42,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}


AI_API_ENTRY
ai_error ai_word_classifier_get_error(ai_handle network)
{
  return ai_platform_network_get_error(network);
}


AI_API_ENTRY
ai_error ai_word_classifier_create(
  ai_handle* network, const ai_buffer* network_config)
{
  return ai_platform_network_create(
    network, network_config, 
    AI_CONTEXT_OBJ(&AI_NET_OBJ_INSTANCE),
    AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR, AI_TOOLS_API_VERSION_MICRO);
}


AI_API_ENTRY
ai_error ai_word_classifier_create_and_init(
  ai_handle* network, const ai_handle activations[], const ai_handle weights[])
{
  ai_error err;
  ai_network_params params;

  err = ai_word_classifier_create(network, AI_WORD_CLASSIFIER_DATA_CONFIG);
  if (err.type != AI_ERROR_NONE) {
    return err;
  }
  
  if (ai_word_classifier_data_params_get(&params) != true) {
    err = ai_word_classifier_get_error(*network);
    return err;
  }
#if defined(AI_WORD_CLASSIFIER_DATA_ACTIVATIONS_COUNT)
  /* set the addresses of the activations buffers */
  for (ai_u16 idx=0; activations && idx<params.map_activations.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_activations, idx, activations[idx]);
  }
#endif
#if defined(AI_WORD_CLASSIFIER_DATA_WEIGHTS_COUNT)
  /* set the addresses of the weight buffers */
  for (ai_u16 idx=0; weights && idx<params.map_weights.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_weights, idx, weights[idx]);
  }
#endif
  if (ai_word_classifier_init(*network, &params) != true) {
    err = ai_word_classifier_get_error(*network);
  }
  return err;
}


AI_API_ENTRY
ai_buffer* ai_word_classifier_inputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_inputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_buffer* ai_word_classifier_outputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_outputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_handle ai_word_classifier_destroy(ai_handle network)
{
  return ai_platform_network_destroy(network);
}


AI_API_ENTRY
ai_bool ai_word_classifier_init(
  ai_handle network, const ai_network_params* params)
{
  ai_network* net_ctx = AI_NETWORK_OBJ(ai_platform_network_init(network, params));
  ai_bool ok = true;

  if (!net_ctx) return false;
  ok &= word_classifier_configure_weights(net_ctx, params);
  ok &= word_classifier_configure_activations(net_ctx, params);

  ok &= ai_platform_network_post_init(network);

  return ok;
}


AI_API_ENTRY
ai_i32 ai_word_classifier_run(
  ai_handle network, const ai_buffer* input, ai_buffer* output)
{
  return ai_platform_network_process(network, input, output);
}


AI_API_ENTRY
ai_i32 ai_word_classifier_forward(ai_handle network, const ai_buffer* input)
{
  return ai_platform_network_process(network, input, NULL);
}



#undef AI_WORD_CLASSIFIER_MODEL_SIGNATURE
#undef AI_NET_OBJ_INSTANCE
#undef AI_TOOLS_DATE_TIME
#undef AI_TOOLS_COMPILE_TIME

