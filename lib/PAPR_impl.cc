/* -*- c++ -*- */
/*
 * Copyright 2023 Deniz Unal.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <volk/volk.h>

#include "PAPR_impl.h"

namespace gr {
namespace uwphy {

PAPR::sptr PAPR::make(size_t vlen) {
  return gnuradio::get_initial_sptr(new PAPR_impl(vlen));
}

PAPR_impl::PAPR_impl(size_t vlen)
    : gr::sync_block("PAPR",
                     gr::io_signature::make(1, 1, sizeof(gr_complex) * vlen),
                     gr::io_signature::make(1, 1, sizeof(float))),
      d_vlen(vlen) {
  const int alignment_multiple = volk_get_alignment() / sizeof(gr_complex);
  set_alignment(std::max(1, alignment_multiple));
}

PAPR_impl::~PAPR_impl() {}

int PAPR_impl::work(int noutput_items, gr_vector_const_void_star &input_items,
                    gr_vector_void_star &output_items) {
  const gr_complex *in = (const gr_complex *)input_items[0];
  float *out = (float *)output_items[0];
  float *power =
      (float *)volk_malloc(sizeof(float) * d_vlen, volk_get_alignment());
  uint32_t i_peak;
  float avg;

  volk_32fc_magnitude_squared_32f(power, in, d_vlen);
  volk_32f_accumulator_s32f(&avg, power, d_vlen);
  volk_32f_index_max_32u(&i_peak, power, d_vlen);
  // volk_32f_index_max_32u_manual(&i_peak, power, d_vlen, "generic");

  avg = avg / d_vlen;
  out[0] = 10.0f * log10(power[i_peak] / avg);

  return 1;
}

} /* namespace uwphy */
} /* namespace gr */
