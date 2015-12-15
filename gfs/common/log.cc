//---------------------------------------------------------- -*- Mode: C++ -*-
// $Id: log.cc 385 2010-05-27 15:58:30Z sriramsrao $
//
// Created 2005/03/01
//
// Copyright 2008 Quantcast Corp.
// Copyright 2006-2008 Kosmix Corp.
//
// This file is part of Kosmos File System (KFS).
//
// Licensed under the Apache License, Version 2.0
// (the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
// implied. See the License for the specific language governing
// permissions and limitations under the License.
// 
//----------------------------------------------------------------------------

#include "log.h"
#include <stdio.h>
#include <limits>

namespace KFS
{

MsgLogger* KFS::MsgLogger::logger = 0;

MsgLogger::MsgLogger(
    const char*         filename,
    MsgLogger::LogLevel logLevel)
    : BufferedLogWriter(filename ? -1 : fileno(stderr), filename)
{
    BufferedLogWriter::SetLogLevel(logLevel);
    BufferedLogWriter::SetMaxLogWaitTime(std::numeric_limits<int>::max());
}

MsgLogger::~MsgLogger()
{
    if (this == logger) {
        logger = 0;
    }
}

void
MsgLogger::Init(
    const char*         filename,
    MsgLogger::LogLevel logLevel)
{
    if (! logger) {
        static MsgLogger sLogger(filename, logLevel);
        logger = &sLogger;
    }
}

void
MsgLogger::Init(
    const Properties& props,
    const char*       propPrefix)
{
    Init(0);
    logger->SetParameters(props, propPrefix);
}

void
MsgLogger::Stop()
{
    if (logger) {
        logger->BufferedLogWriter::Stop();
    }
}

}
