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

#include "Packet_Buffer_impl.h"

namespace gr {
namespace uwphy {

Packet_Buffer::sptr Packet_Buffer::make(size_t bufsize) {
  return gnuradio::get_initial_sptr(new Packet_Buffer_impl(bufsize));
}

/*
 * The private constructor
 */
Packet_Buffer_impl::Packet_Buffer_impl(size_t bufsize)
    : gr::sync_block("Packet_Buffer",
                     gr::io_signature::make(1, 1, sizeof(char)),
                     gr::io_signature::make(1, 1, sizeof(char))) {
  set_output_multiple(bufsize);
  d_logger->info("%d samples/packet", bufsize);
}

/*
 * Our virtual destructor.
 */
Packet_Buffer_impl::~Packet_Buffer_impl() {}

int Packet_Buffer_impl::work(int noutput_items,
                             gr_vector_const_void_star &input_items,
                             gr_vector_void_star &output_items) {
  const char *in = (const char *)input_items[0];
  char *out = (char *)output_items[0];

  memcpy(out, in, sizeof(char) * noutput_items);
  d_logger->info("Packet");

  return noutput_items;
}

} /* namespace uwphy */
} /* namespace gr */
