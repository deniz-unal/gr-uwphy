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

#ifndef LIB_PACKET_BUFFER_IMPL_H_
#define LIB_PACKET_BUFFER_IMPL_H_

#include <uwphy/Packet_Buffer.h>

namespace gr {
namespace uwphy {

class Packet_Buffer_impl : public Packet_Buffer {
 private:
  size_t _bufsize;

 public:
  Packet_Buffer_impl(size_t bufsize);
  ~Packet_Buffer_impl();

  int work(int noutput_items, gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
};

}  // namespace uwphy
}  // namespace gr

#endif  // LIB_PACKET_BUFFER_IMPL_H_
