/* -*- c++ -*- */

#define UWPHY_API

%include "gnuradio.i"           // the common stuff

//load generated python docstrings
%include "uwphy_swig_doc.i"

%{
#include "uwphy/Vector_Normalizer.h"
#include "uwphy/Differential_Decoder.h"
#include "uwphy/Differential_Encoder.h"
#include "uwphy/PAPR.h"
#include "uwphy/Packet_Buffer.h"
#include "uwphy/Framer.h"
#include "uwphy/Deframer.h"
%}

%include "uwphy/Vector_Normalizer.h"
GR_SWIG_BLOCK_MAGIC2(uwphy, Vector_Normalizer);
%include "uwphy/Differential_Decoder.h"
GR_SWIG_BLOCK_MAGIC2(uwphy, Differential_Decoder);
%include "uwphy/Differential_Encoder.h"
GR_SWIG_BLOCK_MAGIC2(uwphy, Differential_Encoder);

%include "uwphy/PAPR.h"
GR_SWIG_BLOCK_MAGIC2(uwphy, PAPR);
%include "uwphy/Packet_Buffer.h"
GR_SWIG_BLOCK_MAGIC2(uwphy, Packet_Buffer);
%include "uwphy/Framer.h"
GR_SWIG_BLOCK_MAGIC2(uwphy, Framer);
%include "uwphy/Deframer.h"
GR_SWIG_BLOCK_MAGIC2(uwphy, Deframer);
