/*
 * Copyright (C) 2016 FH Campus Wien
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_at86rf2xx
 * @{
 *
 * @file
 * @brief       Low-level random number generator driver implementation
 *
 * @author      Mathias Tausig <mathias.tausig@fh-campuswien.ac.at>
 *
 * @}
 */

#include "at86rf2xx.h"
#include "at86rf2xx_params.h"
#include "at86rf2xx_internal.h"
#include "periph/hwrng.h"

static at86rf2xx_t dev;

void hwrng_init(void)
{
  const at86rf2xx_params_t *p = &at86rf2xx_params[0];
  netdev2_t *nd = (netdev2_t *)(&dev);
  at86rf2xx_setup(&dev, (at86rf2xx_params_t*)p);
  nd->driver->init(nd);
}

void hwrng_read(uint8_t *buf, unsigned int num)
{
  at86rf2xx_get_random(&dev, buf, num);
}
