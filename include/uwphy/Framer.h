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

#ifndef INCLUDE_UWPHY_FRAMER_H_
#define INCLUDE_UWPHY_FRAMER_H_

#include <gnuradio/block.h>
#include <uwphy/api.h>

namespace gr {
namespace uwphy {

/*!
 * \brief <+description of block+>
 * \ingroup uwphy
 *
 */
class UWPHY_API Framer : virtual public gr::block {
 public:
  typedef boost::shared_ptr<Framer> sptr;

  /*!
   * \brief Return a shared_ptr to a new instance of uwphy::Framer.
   *
   * To avoid accidental use of raw pointers, uwphy::Framer's
   * constructor is in a private implementation
   * class. uwphy::Framer::make is the public interface for
   * creating new instances.
   * \param pre_len   Preamble length in samples
   * \param sym_len   Symbol length in samples
   * \param post_len  Postamble length in samples
   */
  static sptr make(size_t pre_len, size_t sym_len, size_t post_len);
};

}  // namespace uwphy
}  // namespace gr

#endif  // INCLUDE_UWPHY_FRAMER_H_
