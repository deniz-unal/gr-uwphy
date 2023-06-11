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

#include "Framer_impl.h"

namespace gr {
namespace uwphy {

Framer::sptr Framer::make(size_t pre_len, size_t sym_len, size_t post_len) {
  return gnuradio::get_initial_sptr(
      new Framer_impl(pre_len, sym_len, post_len));
}

Framer_impl::Framer_impl(size_t pre_len, size_t sym_len, size_t post_len)
    : gr::block("Framer", gr::io_signature::make(2, 3, sizeof(gr_complex)),
                gr::io_signature::make(1, 1, sizeof(gr_complex))),
      pre_len_(pre_len),
      sym_len_(sym_len),
      post_len_(post_len) {
  set_output_multiple(pre_len + sym_len + post_len);
  d_logger->info("Pre: %d; Sym: %d; Post: %d; Total: %d samples", pre_len_,
                 sym_len_, post_len_, (pre_len_ + sym_len_ + post_len_));
}

Framer_impl::~Framer_impl() {}

void Framer_impl::forecast(int noutput_items,
                           gr_vector_int &ninput_items_required) {
  ninput_items_required[0] = pre_len_;
  ninput_items_required[1] = sym_len_;
  ninput_items_required[2] = post_len_;
}

int Framer_impl::general_work(int noutput_items, gr_vector_int &ninput_items,
                              gr_vector_const_void_star &input_items,
                              gr_vector_void_star &output_items) {
  const gr_complex *pre = (const gr_complex *)input_items[0];
  const gr_complex *sym = (const gr_complex *)input_items[1];
  const gr_complex *post = (const gr_complex *)input_items[2];
  gr_complex *out = (gr_complex *)output_items[0];

  d_logger->info("Framer noutput_items:%d", noutput_items);

  memcpy(out, pre, pre_len_ * sizeof(gr_complex));
  out += pre_len_;
  memcpy(out, sym, sym_len_ * sizeof(gr_complex));
  out += sym_len_;
  memcpy(out, post, post_len_ * sizeof(gr_complex));
  out += post_len_;

  // consume_each(noutput_items);
  this->consume(0, pre_len_);
  this->consume(1, sym_len_);
  this->consume(2, post_len_);

  return pre_len_ + sym_len_ + post_len_;
  // return noutput_items;
}

} /* namespace uwphy */
} /* namespace gr */
