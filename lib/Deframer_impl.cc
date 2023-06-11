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

#include "Deframer_impl.h"
#include "volk/volk.h"

namespace gr {
namespace uwphy {

Deframer::sptr Deframer::make(size_t sym_offset, size_t sym_len,
                              size_t post_len) {
  return gnuradio::get_initial_sptr(
      new Deframer_impl(sym_offset, sym_len, post_len));
}

Deframer_impl::Deframer_impl(size_t sym_offset, size_t sym_len, size_t post_len)
    : gr::block(
          "Deframer",
          gr::io_signature::make2(2, 2, sizeof(gr_complex), sizeof(float)),
          gr::io_signature::make(1, 1, sizeof(gr_complex))),
      sym_offset_(sym_offset),
      sym_len_(sym_len),
      post_len_(post_len),
      state_(0),
      window_start_(0) {
  const int alignment_multiple = volk_get_alignment() / sizeof(gr_complex);
  set_alignment(std::max(1, alignment_multiple));
}

Deframer_impl::~Deframer_impl() {}

void Deframer_impl::forecast(int noutput_items,
                             gr_vector_int &ninput_items_required) {
  ninput_items_required[0] = noutput_items;
  ninput_items_required[1] = noutput_items;
}

// Window overlaps with symbol
bool Deframer_impl::has_overlap(int window[], int symbol[]) {
  if (window[0] <= symbol[1] && symbol[0] <= window[1])
    return true;
  else
    return false;
}

// Return overlap start, end, length
void Deframer_impl::calc_overlap(int window[], int symbol[], int overlap[]) {
  overlap[0] = std::max(window[0], symbol[0]);
  overlap[1] = std::min(window[1], symbol[1]);
  overlap[2] = overlap[1] - overlap[0] + 1;
}

int Deframer_impl::general_work(int noutput_items, gr_vector_int &ninput_items,
                                gr_vector_const_void_star &input_items,
                                gr_vector_void_star &output_items) {
  const gr_complex *in_sig = (const gr_complex *)input_items[0];
  const float *in_det = (const float *)input_items[1];
  gr_complex *out = reinterpret_cast<gr_complex *>(output_items[0]);

  uint32_t max_sample;

  int r_window[2];
  int r_symbol[3];
  r_symbol[0] = sym_offset_;
  r_symbol[1] = sym_offset_ + sym_len_ - 1;
  r_symbol[2] = r_symbol[1] + post_len_;
  int r_overlap[3];

  int overlapSize = 0;

  if (state_ == 0) {
    // Detection mode
    // Find max on detection stream
    volk_32f_index_max_32u_manual(&max_sample, in_det, ninput_items[1],
                                  "generic");
    if (in_det[max_sample] > 0) {
      // Packet detected
      d_logger->debug("Packet detected");
      state_ = 1;
      window_start_ = -max_sample;
    } else {
      // Packet not detected
      // d_logger->trace("Packet not detected");
      state_ = 0;
      consume_each(std::min(ninput_items[0], ninput_items[1]));
    }
  }

  // Relative window position
  r_window[0] = window_start_;
  r_window[1] = window_start_ + std::min(ninput_items[0], ninput_items[1]) - 1;

  if (state_ == 1) {
    // Deframing mode
    // d_logger->trace("Deframing");
    // Check overlap
    if (Deframer_impl::has_overlap(r_window, r_symbol)) {
      // Symbol in window
      Deframer_impl::calc_overlap(r_window, r_symbol, r_overlap);
      overlapSize = r_overlap[2];
      // d_logger->trace("Symbol: %d -> %d #:%d", r_overlap[0], r_overlap[1],
      //                 overlapSize);
      memcpy(out, in_sig + r_overlap[0] - r_window[0],
             sizeof(gr_complex) * overlapSize);
      if (r_overlap[0] != 0) {
        add_item_tag(0, nitems_written(0), pmt::mp("sym_start"),
                     pmt::from_long(sym_len_));
      }
    } else {
      // d_logger->trace("No overlap: %d -> %d", r_window[0], r_window[1]);
    }

    if (r_window[1] > r_symbol[2]) {
      // End of packet
      // d_logger->trace("End of packet");
      state_ = 0;
      consume_each(r_symbol[1] - r_window[0] + 1);
    } else {
      consume_each(std::min(ninput_items[0], ninput_items[1]));
    }
    // Move window pointer
    window_start_ = r_window[1] + 1;
  }

  return overlapSize;
}

} /* namespace uwphy */
} /* namespace gr */
