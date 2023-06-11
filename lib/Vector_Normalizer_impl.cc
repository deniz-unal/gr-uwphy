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

#include "Vector_Normalizer_impl.h"

namespace gr {
namespace uwphy {

Vector_Normalizer::sptr Vector_Normalizer::make(size_t vlen) {
  return gnuradio::get_initial_sptr(new Vector_Normalizer_impl(vlen));
}

/*
 * The private constructor
 */
Vector_Normalizer_impl::Vector_Normalizer_impl(size_t vlen)
    : gr::sync_block("Vector_Normalizer",
                     gr::io_signature::make(1, 1, sizeof(gr_complex) * vlen),
                     gr::io_signature::make(1, 1, sizeof(gr_complex) * vlen)),
      d_vlen(vlen) {
  const int alignment_multiple = volk_get_alignment() / sizeof(gr_complex);
  set_alignment(std::max(1, alignment_multiple));
}

/*
 * Our virtual destructor.
 */
Vector_Normalizer_impl::~Vector_Normalizer_impl() {}

int Vector_Normalizer_impl::work(int noutput_items,
                                 gr_vector_const_void_star &input_items,
                                 gr_vector_void_star &output_items) {
  const gr_complex *in = (gr_complex *)input_items[0];
  gr_complex *out = reinterpret_cast<gr_complex *>(output_items[0]);

  uint32_t max_sample;

  // printf("%d %d\n",d_vlen,sizeof(gr_complex));

  // // Single
  // volk_32fc_index_max_32u_manual(&max_sample, (gr_complex *)in, d_vlen,
  // "generic");
  // // volk_32fc_index_max_32u(&max_sample, (gr_complex *)in, d_vlen+4);
  // printf("maximum is %1.2f at index %u\n", std::abs(in[max_sample]),
  // max_sample); lv_32fc_t scalar = 1 / std::abs(in[max_sample]);
  // volk_32fc_s32fc_multiply_32fc(out, in, scalar, d_vlen);
  // return 1;

  for (int iVec = 0; iVec < noutput_items; iVec++) {
    gr_complex *in_vector = (gr_complex *)in + iVec * d_vlen;
    // TODO: What is this generic? Why it needs it?
    volk_32fc_index_max_32u_manual(&max_sample, in_vector, d_vlen, "generic");
    lv_32fc_t scalar = 1 / std::abs(in_vector[max_sample]);

    // Debug Print
    // printf("%d/%d max: %1.2f index: %u\n", iVec, noutput_items,
    // std::abs(in_vector[max_sample]), max_sample);

    volk_32fc_s32fc_multiply_32fc(out + iVec * d_vlen, in_vector, scalar,
                                  d_vlen);
  }

  return noutput_items;
}

} /* namespace uwphy */
} /* namespace gr */
