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
            sym_offset,
            sym_length,
            post_length,
            src_data_sig,
            src_data_det):
        gr.top_block.__init__(self, "QA Deframer")

        self.src_sig = blocks.vector_source_c(src_data_sig)
        self.src_det = blocks.vector_source_f(src_data_det)
        self.uut = uwphy.Deframer(sym_offset=sym_offset, sym_len=sym_length, post_len=post_length)
        self.dst = blocks.vector_sink_c()
        self.connect(self.src_sig, (self.uut,0))
        self.connect(self.src_det, (self.uut,1))
        self.connect(self.uut, self.dst)

    def get_output(self):
        return self.dst.data()

class qa_Deframer(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    # def generate_src():


    def test_001_t(self):

        sym_offset = 20
        sym_length = 8000
        post_length = 0
        detection = 9641

        # Test data
        src_data_det = np.zeros(30000,dtype=np.float32)
        src_data_det[detection] = 1
        # src_data_sig = np.zeros(100,dtype=np.complex64)
        src_data_sig = np.arange(30000,dtype=np.complex64)
        # src_data_sig[(detection+sym_offset):(detection+sym_offset+sym_length)] = 1

        expected_output = src_data_sig[(detection+sym_offset):(detection+sym_offset+sym_length)]            

        # Flowgraph setup
        fgut = flowgraph(sym_offset=sym_offset, sym_length=sym_length, post_length=post_length, src_data_sig=src_data_sig, src_data_det=src_data_det)

        # Run test
        fgut.run()
        result_data = fgut.get_output()
        self.assertEqual(tuple(expected_output), result_data)


    def test_002_t(self):
        # No packet
        sym_offset = 500
        sym_length = 8000
        post_length = 100

        # Test data
        src_data_det = np.zeros(30000,dtype=np.float32)
        src_data_sig = np.zeros(30000,dtype=np.complex64)

        expected_output = []   

        # Flowgraph setup
        fgut = flowgraph(sym_offset=sym_offset, sym_length=sym_length, post_length=post_length, src_data_sig=src_data_sig, src_data_det=src_data_det)

        # Run test
        fgut.run()
        result_data = fgut.get_output()
        self.assertEqual(tuple(expected_output), result_data)


    def test_003_t(self):
        # Detection during packet
        sym_offset = 20
        sym_length = 8000
        post_length = 0
        detection = 9641

        # Test data
        src_data_det = np.zeros(30000,dtype=np.float32)
        src_data_det[detection] = 1
        src_data_det[detection + 10] = 1
        src_data_det[detection + 20] = 1
        src_data_det[detection + 100] = 1
        # src_data_sig = np.zeros(100,dtype=np.complex64)
        src_data_sig = np.arange(30000,dtype=np.complex64)
        # src_data_sig[(detection+sym_offset):(detection+sym_offset+sym_length)] = 1

        expected_output = src_data_sig[(detection+sym_offset):(detection+sym_offset+sym_length)]            

        # Flowgraph setup
        fgut = flowgraph(sym_offset=sym_offset, sym_length=sym_length, post_length=post_length, src_data_sig=src_data_sig, src_data_det=src_data_det)

        # Run test
        fgut.run()
        result_data = fgut.get_output()
        self.assertEqual(tuple(expected_output), result_data)

    def test_004_t(self):
        
        sym_offset = 10000
        sym_length = 22000
        post_length = 0
        detection = 9641

        # Test data
        src_data_det = np.zeros(50000,dtype=np.float32)
        src_data_det[detection] = 1
        # src_data_sig = np.zeros(100,dtype=np.complex64)
        src_data_sig = np.arange(50000,dtype=np.complex64)
        # src_data_sig[(detection+sym_offset):(detection+sym_offset+sym_length)] = 1

        expected_output = src_data_sig[(detection+sym_offset):(detection+sym_offset+sym_length)]            

        # Flowgraph setup
        fgut = flowgraph(sym_offset=sym_offset, sym_length=sym_length, post_length=post_length, src_data_sig=src_data_sig, src_data_det=src_data_det)

        # Run test
        fgut.run()
        result_data = fgut.get_output()
        self.assertEqual(tuple(expected_output), result_data)

if __name__ == '__main__':
    gr_unittest.run(qa_Deframer)
