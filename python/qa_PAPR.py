#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2023 Deniz Unal.
#
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
#

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import uwphy_swig as uwphy
import numpy as np

class qa_PAPR(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def test_001_t(self):
        vlen = 8192
        K = 10

        # Test data
        src_data = np.random.uniform(-1, 1, [K,vlen]) + 1.j * np.random.uniform(-1, 1, [K,vlen])
        src_mag = np.square(np.abs(src_data))
        expected_output = np.zeros(K,dtype=np.float64)
        for k in range(K):
            expected_output[k] = 10*np.log10(np.max(src_mag[k,:])/np.mean(src_mag[k,:]))

        # Flowgraph setup
        src = blocks.vector_source_c(src_data.flatten(), vlen=vlen)
        uut = uwphy.PAPR(vlen=vlen)
        dst = blocks.vector_sink_f()
        self.tb.connect(src, uut)
        self.tb.connect(uut, dst)

        self.tb.run()
        result_data = dst.data()
        self.assertComplexTuplesAlmostEqual(expected_output, result_data, 3)


if __name__ == '__main__':
    gr_unittest.run(qa_PAPR)
