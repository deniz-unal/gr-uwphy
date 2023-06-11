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


class qa_Differential_Encoder(gr_unittest.TestCase):
    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def test_001_t(self):
        vlen = 1024
        K = 10
        M = 4

        # Test data
        src_data = np.random.randint(M, size=[K, (vlen - 1)], dtype=np.uint8)
        expected_output = np.zeros([K, vlen], dtype=np.uint8)        
        for i in np.arange(K):
            for k in np.arange(vlen - 1):
                expected_output[i, k + 1] = (src_data[i, k] + expected_output[i, k]) % M

        # Flowgraph setup
        src = blocks.vector_source_b(src_data.flatten(), vlen=(vlen - 1))
        uut = uwphy.Differential_Encoder(vlen=vlen, M=M, pilot=0)
        dst = blocks.vector_sink_b(vlen=vlen)
        self.tb.connect(src, uut)
        self.tb.connect(uut, dst)

        self.tb.run()
        result_data = dst.data()

        self.assertEqual(expected_output.flatten().tolist(), list(result_data), 6)


if __name__ == "__main__":
    gr_unittest.run(qa_Differential_Encoder)
