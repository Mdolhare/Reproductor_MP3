#ifndef SIN_H
#define SIN_H

#include <stdint.h>

#define NUM_SAMPLES 1024

int16_t sine_wave[] = {
    16383,
    6620,
    7682,
    16349,
    9434,
    10090,
    -341,
    3174,
    21957,
    10988,
    10761,
    18080,
    9786,
    9061,
    -2724,
    -508,
    17055,
    4978,
    3775,
    10274,
    1332,
    146,
    -11901,
    -9743,
    7968,
    -3756,
    -4412,
    2818,
    -5223,
    -5360,
    -16235,
    -12804,
    6251,
    -4090,
    -3353,
    5245,
    -1481,
    -388,
    -10146,
    -5738,
    14132,
    4422,
    5593,
    14418,
    7699,
    8583,
    -1599,
    2176,
    21218,
    10501,
    10506,
    18030,
    9903,
    9299,
    -2417,
    -192,
    17315,
    5113,
    3713,
    9940,
    651,
    -954,
    -13494,
    -11897,
    5192,
    -7214,
    -8600,
    -2142,
    -10989,
    -11953,
    -23667,
    -21075,
    -2845,
    -13991,
    -14021,
    -6140,
    -13526,
    -13021,
    -23284,
    -19290,
    267,
    -9646,
    -8564,
    293,
    -6266,
    -5097,
    -14864,
    -10545,
    9167,
    -755,
    161,
    8708,
    1708,
    2326,
    -8086,
    -4483,
    14461,
    3742,
    3856,
    11615,
    3861,
    3777,
    -7269,
    -4219,
    14264,
    3182,
    3036,
    10639,
    2830,
    2788,
    -8128,
    -4871,
    13885,
    3125,
    3336,
    11313,
    3880,
    4196,
    -6393,
    -2858,
    16116,
    5502,
    5777,
    13730,
    6181,
    6288,
    -4600,
    -1449,
    17062,
    5918,
    5609,
    12938,
    4741,
    4194,
    -7337,
    -4799,
    13146,
    1500,
    769,
    7769,
    -651,
    -1309,
    -12832,
    -10163,
    8035,
    -3239,
    -3488,
    4095,
    -3655,
    -3568,
    -14293,
    -10791,
    8253,
    -2182,
    -1623,
    6714,
    -354,
    319,
    -9930,
    -6080,
    13173,
    2796,
    3259,
    11345,
    3868,
    3984,
    -6962,
    -3935,
    14384,
    2984,
    2357,
    9309,
    683,
    -337,
    -12383,
    -10388,
    6994,
    -5225,
    -6530,
    -92,
    -9055,
    -10220,
    -22207,
    -19947,
    -2092,
    -13636,
    -14070,
    -6580,
    -14326,
    -14131,
    -24638,
    -20808,
    -1320,
    -11201,
    -9975,
    -860,
    -7047,
    -5392,
    -14569,
    -9562,
    10922,
    1839,
    3646,
    13116,
    7047,
    8583,
    -947,
    3478,
    23165,
    13085,
    13716,
    21854,
    14326,
    14303,
    3148,
    5912,
    23937,
    12224,
    11285,
    17943,
    9055,
    7816,
    -4392,
    -2498,
    14850,
    2664,
    1460,
    8061,
    -683,
    -1586,
    -13279,
    -10707,
    7459,
    -3787,
    -3959,
    3739,
    -3868,
    -3621,
    -14181,
    -10519,
    8671,
    -1642,
    -991,
    7404,
    354,
    1008,
    -9298,
    -5541,
    13590,
    3068,
    3371,
    11291,
    3655,
    3628,
    -7433,
    -4483,
    13809,
    2440,
    1910,
    9032,
    651,
    -46,
    -11691,
    -9224,
    8698,
    -2923,
    -3584,
    3528,
    -4741,
    -5215,
    -16531,
    -13641,
    4782,
    -6273,
    -6321,
    1434,
    -6181,
    -6007,
    -16699,
    -13225,
    5728,
    -4865,
    -4528,
    3526,
    -3880,
    -3590,
    -14258,
    -10848,
    7959,
    -2851,
    -2794,
    4934,
    -2830,
    -2916,
    -13958,
    -10905,
    7580,
    -3503,
    -3653,
    3945,
    -3861,
    -3892,
    -14778,
    -11465,
    7382,
    -3239,
    -2836,
    5396,
    -1708,
    -985,
    -11083,
    -6967,
    12676,
    2822,
    3942,
    12820,
    6266,
    7429,
    -2358,
    1923,
    21577,
    11567,
    12362,
    20744,
    13526,
    13863,
    3098,
    6267,
    24690,
    13351,
    12744,
    19677,
    10989,
    9865,
    -2321,
    -508,
    16652,
    4173,
    2572,
    8677,
    -651,
    -2216,
    -14635,
    -12836,
    4528,
    -7530,
    -8504,
    -1576,
    -9903,
    -10307,
    -21428,
    -18224,
    625,
    -9899,
    -9323,
    -860,
    -7699,
    -6694,
    -16515,
    -12145,
    7712,
    -1984,
    -775,
    8111,
    1481,
    2477,
    -7568,
    -3632,
    15592,
    5081,
    5312,
    13083,
    5223,
    4904,
    -6510,
    -3966,
    13875,
    2020,
    979,
    7576,
    -1332,
    -2551,
    -14697,
    -12701,
    4788,
    -7214,
    -8197,
    -1338,
    -9786,
    -10357,
    -21683,
    -18711,
    -112,
    -10898,
    -10581,
    -2366,
    -9434,
    -8626,
    -18604,
    -14343,
    5461,
    -4226,
    -2938,
    6102,
    -297,
    1008,
    -8646,
    -4239,
    15537,
    5651,
    6581,
    15115,
    8077,
    8632,
    -1867,
    1621,
    20430,
    9551,
    9482,
    17036,
    9055,
    8724,
    -2589,
    175,
    18356,
    6955,
    6476,
    13732,
    5564,
    5151,
    -6144,
    -3274,
    15088,
    3929,
    3739,
    11313,
    3477,
    3391,
    -7596,
    -4454,
    14132,
    3138,
    3043,
    10637,
    2741,
    2515,
    -8692,
    -5844,
    12382,
    971,
    415,
    7518,
    -884,
    -1614,
    -13306,
    -10905,
    6928,
    -4805,
    -5599,
    1362,
    -7071,
    -7716,
    -19201,
    -16469,
    1816,
    -9345,
    -9457,
    -1713,
    -9280,
    -8989,
    -19487,
    -15738,
    3574,
    -6576,
    -5715,
    2941,
    -3794,
    -2773,
    -12664,
    -8443,
    11189,
    1199,
    2056,
    10540,
    3464,
    3984,
    -6559,
    -3131,
    15587,
    4581,
    4341,
    11673,
    3417,
    2755,
    -8943,
    -6614,
    11086,
    -830,
    -1850,
    4853,
    -3861,
    -4800,
    -16581,
    -14138,
    3876,
    -7530,
    -7852,
    -274,
    -7956,
    -7723,
    -18218,
    -14400,
    5048,
    -4895,
    -3757,
    5245,
    -1078,
    416,
    -8943,
    -4141,
    16116,
    6785,
    8326,
    17510,
    11139,
    12356,
    2492,
    6571,
    25898,
    15448,
    15700,
    23450,
    15529,
    15108,
    3551,
    5912,
    23534,
    11419,
    10082,
    16347,
    7071,
    5452,
    -7125,
    -5590,
    11410,
    -1108,
    -2631,
    3666,
    -5363,
    -6533,
    -18473,
    -16128,
    1833,
    -9596,
    -9929,
    -2366,
    -10086,
    -9928,
    -20551,
    -16927,
    2251,
    -8050,
    -7361,
    1097,
    -5863,
    -5097,
    -15267,
    -11350,
    7964,
    -2352,
    -1822,
    6345,
    -1024,
    -766,
    -11526,
    -8257,
    10369,
    -652,
    -823,
    6668,
    -1332,
    -1643,
    -12894,
    -10028,
    8295,
    -2923,
    -3181,
    4333,
    -3538,
    -3618,
    -14548,
    -11278,
    7515,
    -3180,
    -2881,
    5208,
    -2088,
    -1612,
    -12019,
    -8278,
    10922,
    555,
    1097,
    9335,
    2088,
    2515,
    -8040,
    -4542,
    14328,
    3555,
    3625,
    11342,
    3538,
    3389,
    -7740,
    -4799,
    13549,
    2305,
    1972,
    9366,
    1332,
    1054,
    -10098,
    -7070,
    11475,
    534,
    603,
    8489,
    1024,
    1378,
    -9099,
    -5371,
    13879,
    3626,
    4345,
    12820,
    5863,
    6625,
    -3561,
    326,
    19593,
    9203,
    9628,
    17651,
    10086,
    10090,
    -993,
    1873,
    20010,
    8405,
    7551,
    14256,
    5363,
    4057,
    -8290,
    -6614,
    10434,
    -2132,
    -3796,
    2270,
    -7071,
    -8624,
    -21004,
    -19143,
    -1689,
    -13636,
    -14473,
    -7385,
    -15529,
    -15727,
    -26622,
    -23171,
    -4054,
    -14294,
    -13415,
    -4633,
    -11139,
    -9787,
    -19249,
    -14509,
    5728,
    -3581,
    -1978,
    7307,
    1078,
    2477,
    -7165,
    -2828,
    16795,
    6677,
    7296,
    15447,
    7956,
    7997,
    -3070,
    -192,
    17968,
    6415,
    5659,
    12523,
    3861,
    2869,
    -9072,
    -6892,
    10758,
    -1108,
    -1979,
    4968,
    -3417,
    -3950,
    -15263,
    -12304,
    6256,
    -4591,
    -4362,
    3739,
    -3464,
    -2817,
    -12978,
    -8923,
    10655,
    720,
    1741,
    10497,
    3794,
    4782,
    -5206,
    -1146,
    18270,
    8015,
    8565,
    16712,
    9280,
    9437,
    -1464,
    1621,
    20027,
    8746,
    8279,
    15439,
    7071,
    6360,
    -5322,
    -2917,
    14916,
    3182,
    2384,
    9337,
    884,
    205,
    -11338,
    -8695,
    9462,
    -1878,
    -2229,
    5208,
    -2741,
    -2914,
    -13965,
    -10861,
    7712,
    -3268,
    -3325,
    4331,
    -3477,
    -3590,
    -14661,
    -11653,
    6756,
    -4448,
    -4778,
    2571,
    -5564,
    -6009,
    -17398,
    -14679,
    3488,
    -7898,
    -8333,
    -1000,
    -9055,
    -9312,
    -20404,
    -17274,
    1413,
    -9345,
    -9054,
    -909,
    -8077,
    -7392,
    -17503,
    -13374,
    6307,
    -3484,
    -2275,
    6714,
    297,
    1620,
    -7984,
    -3497,
    16383,
    6620,
    7682,
    16349,
    9434,
    10090,
    -341,
    3174,
    21957,
    10988,
    10761,
    18080,
    9786,
    9061,
    -2724,
    -508,
    17055,
    4978,
    3775,
    10274,
    1332,
    146,
    -11901,
    -9743,
    7968,
    -3756,
    -4412,
    2818,
    -5223,
    -5360,
    -16235,
    -12804,
    6251,
    -4090,
    -3353,
    5245,
    -1481,
    -388,
    -10146,
    -5738,
    14132,
    4422,
    5593,
    14418,
    7699,
    8583,
    -1599,
    2176,
    21218,
    10501,
    10506,
    18030,
    9903,
    9299,
    -2417,
    -192,
    17315,
    5113,
    3713,
    9940,
    651,
    -954,
    -13494,
    -11897,
    5192,
    -7214,
    -8600,
    -2142,
    -10989,
    -11953,
    -23667,
    -21075,
    -2845,
    -13991,
    -14021,
    -6140,
    -13526,
    -13021,
    -23284,
    -19290,
    267,
    -9646,
    -8564,
    293,
    -6266,
    -5097,
    -14864,
    -10545,
    9167,
    -755,
    161,
    8708,
    1708,
    2326,
    -8086,
    -4483,
    14461,
    3742,
    3856,
    11615,
    3861,
    3777,
    -7269,
    -4219,
    14264,
    3182,
    3036,
    10639,
    2830,
    2788,
    -8128,
    -4871,
    13885,
    3125,
    3336,
    11313,
    3880,
    4196,
    -6393,
    -2858,
    16116,
    5502,
    5777,
    13730,
    6181,
    6288,
    -4600,
    -1449,
    17062,
    5918,
    5609,
    12938,
    4741,
    4194,
    -7337,
    -4799,
    13146,
    1500,
    769,
    7769,
    -651,
    -1309,
    -12832,
    -10163,
    8035,
    -3239,
    -3488,
    4095,
    -3655,
    -3568,
    -14293,
    -10791,
    8253,
    -2182,
    -1623,
    6714,
    -354,
    319,
    -9930,
    -6080,
    13173,
    2796,
    3259,
    11345,
    3868,
    3984,
    -6962,
    -3935,
    14384,
    2984,
    2357,
    9309,
    683,
    -337,
    -12383,
    -10388,
    6994,
    -5225,
    -6530,
    -92,
    -9055,
    -10220,
    -22207,
    -19947,
    -2092,
    -13636,
    -14070,
    -6580,
    -14326,
    -14131,
    -24638,
    -20808,
    -1320,
    -11201,
    -9975,
    -860,
    -7047,
    -5392,
    -14569,
    -9562,
    10922,
    1839,
    3646,
    13116,
    7047,
    8583,
    -947,
    3478,
    23165,
    13085,
    13716,
    21854,
    14326,
    14303,
    3148,
    5912,
    23937,
    12224,
    11285,
    17943,
    9055,
    7816,
    -4392,
    -2498,
};

#endif // SIN_H