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

#include "Differential_Decoder_impl.h"

namespace gr {
namespace uwphy {

Differential_Decoder::sptr Differential_Decoder::make(size_t vlen) {
  return gnuradio::get_initial_sptr(new Differential_Decoder_impl(vlen));
}

Differential_Decoder_impl::Differential_Decoder_impl(size_t vlen)
    : gr::sync_block(
          "Differential_Decoder",
          gr::io_signature::make(1, 1, sizeof(gr_complex) * vlen),
          gr::io_signature::make(1, 1, sizeof(gr_complex) * (vlen - 1))),
      d_vlen(vlen) {
  const int alignment_multiple = volk_get_alignment() / sizeof(gr_complex);
  set_alignment(std::max(1, alignment_multiple));
}

Differential_Decoder_impl::~Differential_Decoder_impl() {}

int Differential_Decoder_impl::work(int noutput_items,
                                    gr_vector_const_void_star &input_items,
                                    gr_vector_void_star &output_items) {
  const gr_complex *in = (gr_complex *)input_items[0];
  gr_complex *out = reinterpret_cast<gr_complex *>(output_items[0]);
  // gr_complex *in1 = (gr_complex *)malloc(sizeof(gr_complex) * (d_vlen - 1));

  // // for loop over noutput_items
  // memcpy(in1, in0 + 1, sizeof(gr_complex) * (d_vlen - 1));
  // volk_32fc_x2_multiply_conjugate_32fc(out, in0, in1, d_vlen - 1);

  // free(in1);

  for (int iVec = 0; iVec < noutput_items; iVec++) {
    volk_32fc_x2_multiply_conjugate_32fc(
        out + (iVec * (d_vlen - 1)), in + (iVec * (d_vlen)),
        in + (iVec * (d_vlen)) + 1, d_vlen - 1);
  }

  return noutput_items;
}

} /* namespace uwphy */
} /* namespace gr */
