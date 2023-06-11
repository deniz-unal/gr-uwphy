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

class flowgraph (gr.top_block):
    def __init__(
            self,
            bufsize,
            src_data):
        gr.top_block.__init__(self, "QA Packet Buffer")

        self.src = blocks.vector_source_b(src_data)
        self.uut = uwphy.Packet_Buffer(bufsize=bufsize)
        self.dst = blocks.vector_sink_b()
        self.connect(self.src, self.uut)
        self.connect(self.uut, self.dst)

    def get_output(self):
        return self.dst.data()


class qa_Packet_Buffer(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def generatePattern(self,bufsize,K):
        # Test data
        src_data = np.random.randint(256,size=K,dtype=np.uint8)
        expected_output = tuple(src_data[:(src_data.size//bufsize)*bufsize])
        return src_data, expected_output

    def test_001_t(self):
        bufsize = 512
        K = bufsize - 1
        src_data, expected_output = self.generatePattern(bufsize, K)

        # Flowgraph setup
        fgut = flowgraph(bufsize=bufsize, src_data=src_data)

        # Run test
        fgut.run()
        result_data = fgut.get_output()
        self.assertEqual(expected_output, result_data)

    def test_002_t(self):
        bufsize = 512
        K = bufsize
        src_data, expected_output = self.generatePattern(bufsize, K)

        # Flowgraph setup
        fgut = flowgraph(bufsize=bufsize, src_data=src_data)

        # Run test
        fgut.run()
        result_data = fgut.get_output()
        self.assertEqual(expected_output, result_data)

    def test_003_t(self):
        bufsize = 512
        K = bufsize + 1
        src_data, expected_output = self.generatePattern(bufsize, K)

        # Flowgraph setup
        fgut = flowgraph(bufsize=bufsize, src_data=src_data)

        # Run test
        fgut.run()
        result_data = fgut.get_output()
        self.assertEqual(expected_output, result_data)

    def test_004_t(self):
        bufsize = 8192
        K = bufsize*20+1
        src_data, expected_output = self.generatePattern(bufsize, K)

        # Flowgraph setup
        fgut = flowgraph(bufsize=bufsize, src_data=src_data)

        # Run test
        fgut.run()
        result_data = fgut.get_output()
        self.assertEqual(expected_output, result_data)


if __name__ == '__main__':
    gr_unittest.run(qa_Packet_Buffer)
