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

class qa_Differential_Decoder(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def test_001_t(self):        
        vlen = 8192
        K = 10

        # Test data
        src_data = np.random.uniform(-1, 1, [K,vlen]) + 1.j * np.random.uniform(-1, 1, [K,vlen])
        expected_output = np.zeros([K,vlen-1]) + 1j * np.zeros([K,vlen-1])

        for i in np.arange(K):
            vec = src_data[i]
            expected_output[i] = vec[0:-1]*np.conj(vec[1:])

        # Flowgraph setup
        src = blocks.vector_source_c(src_data.flatten(), vlen=vlen)
        uut = uwphy.Differential_Decoder(vlen=vlen)
        dst = blocks.vector_sink_c(vlen=vlen-1)
        self.tb.connect(src, uut)
        self.tb.connect(uut, dst)
        
        self.tb.run()
        result_data = dst.data()

        # print('%+f%+fj %+f%+fj' % (src_data[0,0].real, src_data[0,0].imag, src_data[0,1].real, src_data[0,1].imag))
        # print('exp: %+f%+fj' % (expected_output[0,0].real, expected_output[0,0].imag))
        # print('res: %+f%+fj' % (result_data[0].real, result_data[0].imag))
        # print(np.shape(expected_output))
        # np.savetxt('rez.test',result_data)
        # np.savetxt('exp.test',expected_output)
        self.assertComplexTuplesAlmostEqual(expected_output.flatten(), result_data, 6)


if __name__ == '__main__':
    gr_unittest.run(qa_Differential_Decoder)
