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

#include "Differential_Encoder_impl.h"

namespace gr {
namespace uwphy {

Differential_Encoder::sptr Differential_Encoder::make(size_t vlen, uint8_t M,
                                                      uint8_t pilot) {
  return gnuradio::get_initial_sptr(
      new Differential_Encoder_impl(vlen, M, pilot));
}

Differential_Encoder_impl::Differential_Encoder_impl(size_t vlen, uint8_t M,
                                                     uint8_t pilot)
    : gr::sync_block(
          "Differential_Encoder",
          gr::io_signature::make(1, 1, sizeof(unsigned char) * (vlen - 1)),
          gr::io_signature::make(1, 1, sizeof(unsigned char) * vlen)),
      d_vlen(vlen),
      _M(M),
      _pilot(pilot) {}

Differential_Encoder_impl::~Differential_Encoder_impl() {}

int Differential_Encoder_impl::work(int noutput_items,
                                    gr_vector_const_void_star &input_items,
                                    gr_vector_void_star &output_items) {
  const unsigned char *in = (const unsigned char *)input_items[0];
  unsigned char *out = (unsigned char *)output_items[0];

  const unsigned char *inVec;
  unsigned char *outVec;

  for (int iVec = 0; iVec < noutput_items; iVec++) {
    inVec = in + (iVec * (d_vlen - 1));
    outVec = out + (iVec * d_vlen);
    outVec[0] = _pilot;
    for (unsigned int i = 0; i < d_vlen - 1; i++) {
      outVec[i + 1] = (inVec[i] + outVec[i]) % _M;
    }
  }

  return noutput_items;
}

} /* namespace uwphy */
} /* namespace gr */
