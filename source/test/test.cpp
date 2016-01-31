﻿/*
* breeze License
* Copyright (C) 2014-2015 YaweiZhang <yawei.zhang@foxmail.com>.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/


//! 测试

#include <common.h>
#include <utls.h>
using namespace zsummer::log4z;
using namespace zsummer::mysql;

int checkString();
int checkFile();
int checkTime();
int checkFloat();

int main(int argc, char* argv[])
{

#ifndef _WIN32
    //! linux下需要屏蔽的一些信号
    signal( SIGHUP, SIG_IGN );
    signal( SIGALRM, SIG_IGN ); 
    signal( SIGPIPE, SIG_IGN );
    signal( SIGXCPU, SIG_IGN );
    signal( SIGXFSZ, SIG_IGN );
    signal( SIGPROF, SIG_IGN ); 
    signal( SIGVTALRM, SIG_IGN );
    signal( SIGQUIT, SIG_IGN );
    signal( SIGCHLD, SIG_IGN);
    //setenv("TZ", "GMT-8", 1);
#else
    //system("chcp 65001");
#endif

    ILog4zManager::getPtr()->start();
    
    //test trim, split
    if (checkString() != 0)
    {
        return -1;
    }
    if (checkFile() != 0)
    {
        return -1;
    }
    if (checkTime() != 0)
    {
        return -1;
    }
    if (checkFloat() != 0)
    {
        return -1;
    }
    LOGA("check all success.");
    sleepMillisecond(3000);
    return 0;
}



int checkString()
{
    LOGA("begin check trim, splitString, toUpperString,toLowerString,compareStringIgnCase");
    std::string t1 = "%^&123^=&";
    auto ret = splitString(t1, "=", "&%^");
    if (ret.size() != 2)
    {
        return -1;
    }
    if (ret.front() != "123")
    {
        return -1;
    }
    if (!ret.back().empty())
    {
        return -1;
    }
    if (splitString("==", "==", "adf123").size() != 2)
    {
        return -1;
    }
    if (!compareStringIgnCase("Fad123123", "fAd123123"))
    {
        return -1;
    }
    if (compareStringIgnCase("1234", "123", true))
    {
        return -1;
    }
    if (!compareStringIgnCase("a123", "A1234", true))
    {
        return -1;
    }
    if (compareStringIgnCase("a123", "A1234", false))
    {
        return -1;
    }
    if (strcmp(toUpperString("aaaa").c_str(), "AAAA") != 0)
    {
        return -1;
    }
    if (strcmp(toLowerString("AAAA").c_str(), "aaaa") != 0)
    {
        return -1;
    }
    LOGA("end check trim, splitString, toUpperString,toLowerString,compareStringIgnCase");
    return 0;
}

int checkFile()
{
    LOGA("begin check readFileContent, writeFileContent, isDirectory, createRecursionDir, MD5Data genMD5,genFileMD5");
    std::string content = "1234567890";
    std::string path = "./log2/log3/";
    std::string filename = "aaaa";
    std::string md5 = "e807f1fcf82d132f9bb018ca6738a19F";
    if (isDirectory(path))
    {
        return -1;
    }
    if (!createRecursionDir(path))
    {
        return -1;
    }
    if (!isDirectory(path))
    {
        return -1;
    }
    writeFileContent(path + filename, content.c_str(), content.length(), false);
    content.clear();
    size_t lastSize = 0;
    do
    {
        std::string str = readFileContent(path + filename, true, 4, lastSize);
        lastSize += str.length();
        content += str;
        if (str.length() < 4)
        {
            break;
        }

    } while (true);


    MD5Data d;
    d << content;
    std::string mmd5 = d.genMD5();
    if (!compareStringIgnCase(toUpperString(mmd5), toLowerString(md5)))
    {
        return -1;
    }

    if (!compareStringIgnCase(toUpperString(genFileMD5(path + filename)), toLowerString(md5)))
    {
        return -1;
    }
    if (!removeFile(path + filename))
    {
        return -1;
    }
    if (!removeDir(path))
    {
        return -1;
    }
    LOGA("end check readFileContent, writeFileContent, isDirectory, createRecursionDir, MD5Data genMD5,genFileMD5");
    return 0;
}

int checkTime()
{
    LOGA("begin check Time");
    double now = getNow();
    double snow = getSteadyNow();
    long long nowt = getNowTick();
    long long nowst = getSteadyNowTick();
    time_t nowts = getTimestampNow();
    sleepMillisecond(3000);
    now = getNow() - now - 3.0;
    snow = getSteadyNow() - snow - 3.0;
    nowt = getNowTick() - nowt - 3000;
    nowst = getSteadyNowTick() - nowst - 3000;
    nowts = getTimestampNow() - nowts -3;
    if (now > 1 || snow > 1 || nowt >1000 || nowst >1000 || nowts > 1)
    {
        return -1;
    }
    LOGI(getDateString(getTimestampNow()) << " " << getTimeString(getTimestampNow()));
    LOGI(getDateTimeString(getTimestampNow()));

    //2015周四/2016周五
    time_t dt2015 = 1451577599;
    time_t dt2016 = 1451577600;
    if (isSameDay(dt2015, dt2016) || isSameMonth(dt2015, dt2016)
        || isSameYear(dt2015, dt2016))
    {
        return -1;
    }
    if (!isSameDay(dt2015, dt2016, -1) || !isSameMonth(dt2015, dt2016, -1)
        || !isSameYear(dt2015, dt2016, -1))
    {
        return -1;
    }
    if (!isSameWeak(dt2015, dt2016) || isSameWeak(dt2015+3*24*3600, dt2016 + 3*24+3600))
    {
        return -1;
    }
    if (!isSameWeak(dt2015+3*24*3600+1, dt2015+3*24*3600+3))
    {
        return -1;
    }
    
    
    LOGA("end check Time");
    return 0;
}

int checkFloat()
{
    LOGA("begin check float");
    if(isZero(POINT_DOUBLE*2))
    {
        return -1;
    }
    if (!isZero(POINT_DOUBLE / -10.0))
    {
        return -1;
    }
    if (!isZero(POINT_DOUBLE / 10.0))
    {
        return -1;
    }
    if (!isEqual(1.0 + POINT_DOUBLE, 1.0 + POINT_DOUBLE/2.0))
    {
        return -1;
    }
    if (isEqual(1.0 + POINT_DOUBLE*2, 1.0))
    {
        return -1;
    }
    if (!isEqual(getDistance(1.0, -1.0), 2.0))
    {
        return -1;
    }
    if (!isEqual(getDistance(1.0, 0.0, 2.0, 0.0), 1.0))
    {
        return -1;
    }
    if (!isEqual(getRadian(0.0, 0.0, 1.0, 0.0), 0.0))
    {
        return -1;
    }
    if (!isEqual(getRadian(0.0, 0.0, -1.0, 0.0), PI))
    {
        return -1;
    }
    if (!isEqual(std::get<1>(getFarPoint(0.0, 0.0, PI/2.0*3.0, 1.0)), -1.0))
    {
        return -1;
    }
    if (!isEqual(std::get<1>(getFarOffset(0.0, 0.0, 0.0, -2.0, 1.0)), -1.0))
    {
        return -1;
    }
    if (!isEqual(std::get<1>(getFarOffset(0.0, 0.0, 0.0, 2.0, 1.0)), 1.0))
    {
        return -1;
    }
    if (!isEqual(std::get<0>(rotatePoint(0.0, 0.0, PI/2.0, 1.0, PI/2.0)), -1.0))
    {
        return -1;
    }
    if (!isEqual(std::get<1>(rotatePoint(0.0, 0.0, PI / 2.0, 1.0, PI)), -1.0))
    {
        return -1;
    }

    LOGA("end check float");
    return 0;
}






