/*
 * gpt_pwm.c
 *
 *  Created on: Apr 10, 2024
 *      Author: giang.nguyen-hoang
 */

#include "gpt_pwm.h"

void init_gpt_pwm(void)
{
    fsp_err_t err;

    /* Initializes the module. */
    err = R_GPT_Open (&g_timer3_pwm_ctrl, &g_timer3_pwm_cfg);
    /* Handle any errors. This function should be defined by the user. */
    assert (FSP_SUCCESS == err);

    /* Start the timer. */
    (void) R_GPT_Start (&g_timer3_pwm_ctrl);
}

