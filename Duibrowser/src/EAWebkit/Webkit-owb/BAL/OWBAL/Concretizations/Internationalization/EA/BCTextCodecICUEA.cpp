/*
Copyright (C) 2008-2010 Electronic Arts, Inc.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1.  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
3.  Neither the name of Electronic Arts, Inc. ("EA") nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY ELECTRONIC ARTS AND ITS CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ELECTRONIC ARTS OR ITS CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/////////////////////////////////////////////////////////////////////////////
// TextCodecICU.cpp
// Written by Paul Pedriana
/////////////////////////////////////////////////////////////////////////////


#include "config.h"
#include <EABase/eabase.h>
#include "TextCodecICU.h"
#include "CharacterNames.h"
#include "CString.h"
#include "PlatformString.h"
#include "StringBuffer.h"
#include <wtf/Assertions.h>

#if (defined(__GNUC__) && (__GNUC__ >= 3)) || defined(__MWERKS__) || defined(__xlC__)
    #include <alloca.h>
#elif defined(_MSC_VER)
    #include <malloc.h>
#elif (defined(__GNUC__) && (__GNUC__ < 3)) && !defined(alloca)
    #define alloca __builtin_alloca
#endif


using std::auto_ptr;
using std::min;

#include <EAIO/FnEncode.h>

namespace OWBAL {


static auto_ptr<TextCodec> newTextCodecICU(const TextEncoding& encoding, const void*)
{
    // To do: Convert this so it doesn't use the heap but instead uses non-allocated versions.
    //        We can't just make a single global version or even a version per thread, as 
    //        the TextEncoding class has state. - Paul Pedriana
    return auto_ptr<TextCodec>(new TextCodecICU(encoding));
}

void TextCodecICU::registerBaseEncodingNames(EncodingNameRegistrar registrar)
{
    registrar("UTF-8", "UTF-8");
}

void TextCodecICU::registerBaseCodecs(TextCodecRegistrar registrar)
{
    registrar("UTF-8", newTextCodecICU, 0);
}

void TextCodecICU::registerExtendedEncodingNames(EncodingNameRegistrar registrar)
{
    // We register Hebrew with logical ordering using a separate name.
    // Otherwise, this would share the same canonical name as the
    // visual ordering case, and then TextEncoding could not tell them
    // apart; ICU works with either name.

    // Note by Paul Pedriana: I have disabled some of these as they are 
    // very unlikely to be seen in practice and add to the application memory 
    // usage.

    //registrar("ISO-8859-8-I", "ISO-8859-8-I");
    registrar("UTF-8", "UTF-8");
        // registrar("ibm-1208", "UTF-8");
        // registrar("ibm-1209", "UTF-8");
        // registrar("ibm-5304", "UTF-8");
        // registrar("ibm-5305", "UTF-8");
        // registrar("ibm-13496", "UTF-8");
        // registrar("ibm-13497", "UTF-8");
        // registrar("ibm-17592", "UTF-8");
        // registrar("ibm-17593", "UTF-8");
    registrar("windows-65001", "UTF-8");
        // registrar("cp1208", "UTF-8");
    //registrar("UTF-16", "UTF-16");
        // registrar("ISO-10646-UCS-2", "UTF-16");
        // registrar("ibm-1204", "UTF-16");
        // registrar("ibm-1205", "UTF-16");
    //registrar("unicode", "UTF-16");
        // registrar("csUnicode", "UTF-16");
    //registrar("ucs-2", "UTF-16");
    //registrar("UTF-16BE", "UTF-16BE");
        // registrar("x-utf-16be", "UTF-16BE");
        // registrar("ibm-1200", "UTF-16BE");
        // registrar("ibm-1201", "UTF-16BE");
        // registrar("ibm-13488", "UTF-16BE");
        // registrar("ibm-13489", "UTF-16BE");
        // registrar("ibm-17584", "UTF-16BE");
        // registrar("ibm-17585", "UTF-16BE");
        // registrar("ibm-21680", "UTF-16BE");
        // registrar("ibm-21681", "UTF-16BE");
        // registrar("ibm-25776", "UTF-16BE");
        // registrar("ibm-25777", "UTF-16BE");
        // registrar("ibm-61955", "UTF-16BE");
        // registrar("ibm-61956", "UTF-16BE");
        // registrar("windows-1201", "UTF-16BE");
        // registrar("cp1200", "UTF-16BE");
        // registrar("cp1201", "UTF-16BE");
        // registrar("UTF16_BigEndian", "UTF-16BE");
        // registrar("UTF-16LE", "UTF-16LE");
        // registrar("x-utf-16le", "UTF-16LE");
        // registrar("ibm-1202", "UTF-16LE");
        // registrar("ibm-1203", "UTF-16LE");
        // registrar("ibm-13490", "UTF-16LE");
        // registrar("ibm-13491", "UTF-16LE");
        // registrar("ibm-17586", "UTF-16LE");
        // registrar("ibm-17587", "UTF-16LE");
        // registrar("ibm-21682", "UTF-16LE");
        // registrar("ibm-21683", "UTF-16LE");
        // registrar("ibm-25778", "UTF-16LE");
        // registrar("ibm-25779", "UTF-16LE");
        // registrar("UTF16_LittleEndian", "UTF-16LE");
    registrar("windows-1200", "UTF-16LE");
        // registrar("UTF-32", "UTF-32");
        // registrar("ISO-10646-UCS-4", "UTF-32");
        // registrar("ibm-1236", "UTF-32");
        // registrar("ibm-1237", "UTF-32");
        // registrar("csUCS4", "UTF-32");
        // registrar("ucs-4", "UTF-32");
        // registrar("UTF-32BE", "UTF-32BE");
        // registrar("UTF32_BigEndian", "UTF-32BE");
        // registrar("ibm-1232", "UTF-32BE");
        // registrar("ibm-1233", "UTF-32BE");
        // registrar("UTF-32LE", "UTF-32LE");
        // registrar("UTF32_LittleEndian", "UTF-32LE");
        // registrar("ibm-1234", "UTF-32LE");
        // registrar("ibm-1235", "UTF-32LE");
        // registrar("UTF-7", "UTF-7");
        // registrar("windows-65000", "UTF-7");
        // registrar("SCSU", "SCSU");
        // registrar("ibm-1212", "SCSU");
        // registrar("ibm-1213", "SCSU");
        // registrar("BOCU-1", "BOCU-1");
        // registrar("csBOCU-1", "BOCU-1");
        // registrar("ibm-1214", "BOCU-1");
        // registrar("ibm-1215", "BOCU-1");
        // registrar("CESU-8", "CESU-8");
        // registrar("ibm-9400", "CESU-8");
        // registrar("ISO_8859-1:1987", "ISO_8859-1:1987");
        // registrar("ISO-8859-1", "ISO_8859-1:1987");
        // registrar("ibm-819", "ISO_8859-1:1987");
        // registrar("IBM819", "ISO_8859-1:1987");
        // registrar("cp819", "ISO_8859-1:1987");
        // registrar("latin1", "ISO_8859-1:1987");
        // registrar("8859_1", "ISO_8859-1:1987");
        // registrar("csISOLatin1", "ISO_8859-1:1987");
        // registrar("iso-ir-100", "ISO_8859-1:1987");
        // registrar("l1", "ISO_8859-1:1987");
        // registrar("819", "ISO_8859-1:1987");
        // registrar("ANSI_X3.4-1968", "ANSI_X3.4-1968");
        // registrar("US-ASCII", "ANSI_X3.4-1968");
        // registrar("ASCII", "ANSI_X3.4-1968");
        // registrar("ANSI_X3.4-1986", "ANSI_X3.4-1968");
        // registrar("ISO_646.irv:1991", "ANSI_X3.4-1968");
        // registrar("iso_646.irv:1983", "ANSI_X3.4-1968");
        // registrar("ISO646-US", "ANSI_X3.4-1968");
        // registrar("us", "ANSI_X3.4-1968");
        // registrar("csASCII", "ANSI_X3.4-1968");
        // registrar("iso-ir-6", "ANSI_X3.4-1968");
        // registrar("cp367", "ANSI_X3.4-1968");
        // registrar("ascii7", "ANSI_X3.4-1968");
        // registrar("646", "ANSI_X3.4-1968");
        // registrar("windows-20127", "ANSI_X3.4-1968");
        // registrar("ibm-367", "ANSI_X3.4-1968");
        // registrar("IBM367", "ANSI_X3.4-1968");
        // registrar("gb18030", "gb18030");
        // registrar("ibm-1392", "gb18030");
        // registrar("windows-54936", "gb18030");
        // registrar("ISO_8859-2:1987", "ISO_8859-2:1987");
        // registrar("ibm-912_P100-1995", "ISO_8859-2:1987");
        // registrar("ibm-912", "ISO_8859-2:1987");
        // registrar("ISO-8859-2", "ISO_8859-2:1987");
        // registrar("latin2", "ISO_8859-2:1987");
        // registrar("csISOLatin2", "ISO_8859-2:1987");
        // registrar("iso-ir-101", "ISO_8859-2:1987");
        // registrar("l2", "ISO_8859-2:1987");
        // registrar("8859_2", "ISO_8859-2:1987");
        // registrar("cp912", "ISO_8859-2:1987");
        // registrar("912", "ISO_8859-2:1987");
        // registrar("windows-28592", "ISO_8859-2:1987");
        // registrar("ISO_8859-3:1988", "ISO_8859-3:1988");
        // registrar("ibm-913_P100-2000", "ISO_8859-3:1988");
        // registrar("ibm-913", "ISO_8859-3:1988");
        // registrar("ISO-8859-3", "ISO_8859-3:1988");
        // registrar("latin3", "ISO_8859-3:1988");
        // registrar("csISOLatin3", "ISO_8859-3:1988");
        // registrar("iso-ir-109", "ISO_8859-3:1988");
        // registrar("l3", "ISO_8859-3:1988");
        // registrar("8859_3", "ISO_8859-3:1988");
        // registrar("cp913", "ISO_8859-3:1988");
        // registrar("913", "ISO_8859-3:1988");
        // registrar("windows-28593", "ISO_8859-3:1988");
        // registrar("ISO_8859-4:1988", "ISO_8859-4:1988");
        // registrar("ibm-914_P100-1995", "ISO_8859-4:1988");
        // registrar("ibm-914", "ISO_8859-4:1988");
        // registrar("ISO-8859-4", "ISO_8859-4:1988");
        // registrar("latin4", "ISO_8859-4:1988");
        // registrar("csISOLatin4", "ISO_8859-4:1988");
        // registrar("iso-ir-110", "ISO_8859-4:1988");
        // registrar("l4", "ISO_8859-4:1988");
        // registrar("8859_4", "ISO_8859-4:1988");
        // registrar("cp914", "ISO_8859-4:1988");
        // registrar("914", "ISO_8859-4:1988");
        // registrar("windows-28594", "ISO_8859-4:1988");
        // registrar("ISO_8859-5:1988", "ISO_8859-5:1988");
        // registrar("ibm-915_P100-1995", "ISO_8859-5:1988");
        // registrar("ibm-915", "ISO_8859-5:1988");
        // registrar("ISO-8859-5", "ISO_8859-5:1988");
        // registrar("cyrillic", "ISO_8859-5:1988");
        // registrar("csISOLatinCyrillic", "ISO_8859-5:1988");
        // registrar("iso-ir-144", "ISO_8859-5:1988");
        // registrar("8859_5", "ISO_8859-5:1988");
        // registrar("cp915", "ISO_8859-5:1988");
        // registrar("915", "ISO_8859-5:1988");
        // registrar("windows-28595", "ISO_8859-5:1988");
        // registrar("ISO_8859-6:1987", "ISO_8859-6:1987");
        // registrar("ibm-1089_P100-1995", "ISO_8859-6:1987");
        // registrar("ibm-1089", "ISO_8859-6:1987");
        // registrar("ISO-8859-6", "ISO_8859-6:1987");
        // registrar("arabic", "ISO_8859-6:1987");
        // registrar("csISOLatinArabic", "ISO_8859-6:1987");
        // registrar("iso-ir-127", "ISO_8859-6:1987");
        // registrar("ECMA-114", "ISO_8859-6:1987");
        // registrar("ASMO-708", "ISO_8859-6:1987");
        // registrar("8859_6", "ISO_8859-6:1987");
        // registrar("cp1089", "ISO_8859-6:1987");
        // registrar("1089", "ISO_8859-6:1987");
        // registrar("windows-28596", "ISO_8859-6:1987");
        // registrar("ISO-8859-6-I", "ISO_8859-6:1987");
        // registrar("ISO-8859-6-E", "ISO_8859-6:1987");
        // registrar("ISO_8859-7:1987", "ISO_8859-7:1987");
        // registrar("ibm-9005_X110-2007", "ISO_8859-7:1987");
        // registrar("ibm-9005", "ISO_8859-7:1987");
        // registrar("ISO-8859-7", "ISO_8859-7:1987");
        // registrar("greek", "ISO_8859-7:1987");
        // registrar("greek8", "ISO_8859-7:1987");
        // registrar("ELOT_928", "ISO_8859-7:1987");
        // registrar("ECMA-118", "ISO_8859-7:1987");
        // registrar("csISOLatinGreek", "ISO_8859-7:1987");
        // registrar("iso-ir-126", "ISO_8859-7:1987");
        // registrar("windows-28597", "ISO_8859-7:1987");
        // registrar("sun_eu_greek", "ISO_8859-7:1987");
        // registrar("ISO_8859-8:1988", "ISO_8859-8:1988");
        // registrar("ibm-5012_P100-1999", "ISO_8859-8:1988");
        // registrar("ibm-5012", "ISO_8859-8:1988");
        // registrar("ISO-8859-8", "ISO_8859-8:1988");
        // registrar("hebrew", "ISO_8859-8:1988");
        // registrar("csISOLatinHebrew", "ISO_8859-8:1988");
        // registrar("iso-ir-138", "ISO_8859-8:1988");
        // registrar("ISO-8859-8-I", "ISO_8859-8:1988");
        // registrar("ISO-8859-8-E", "ISO_8859-8:1988");
        // registrar("8859_8", "ISO_8859-8:1988");
        // registrar("windows-28598", "ISO_8859-8:1988");
        // registrar("windows-1254", "windows-1254");
        // registrar("ibm-920_P100-1995", "windows-1254");
        // registrar("ibm-920", "windows-1254");
        // registrar("ISO-8859-9", "windows-1254");
        // registrar("latin5", "windows-1254");
        // registrar("csISOLatin5", "windows-1254");
        // registrar("iso-ir-148", "windows-1254");
        // registrar("ISO_8859-9:1989", "windows-1254");
        // registrar("l5", "windows-1254");
        // registrar("8859_9", "windows-1254");
        // registrar("cp920", "windows-1254");
        // registrar("920", "windows-1254");
        // registrar("windows-28599", "windows-1254");
        // registrar("ECMA-128", "windows-1254");
        // registrar("ISO-8859-10", "ISO-8859-10");
        // registrar("iso-8859_10-1998", "ISO-8859-10");
        // registrar("iso-ir-157", "ISO-8859-10");
        // registrar("l6", "ISO-8859-10");
        // registrar("ISO_8859-10:1992", "ISO-8859-10");
        // registrar("csISOLatin6", "ISO-8859-10");
        // registrar("latin6", "ISO-8859-10");
        // registrar("ISO-8859-13", "ISO-8859-13");
        // registrar("ibm-921_P100-1995", "ISO-8859-13");
        // registrar("ibm-921", "ISO-8859-13");
        // registrar("8859_13", "ISO-8859-13");
        // registrar("windows-28603", "ISO-8859-13");
        // registrar("cp921", "ISO-8859-13");
        // registrar("921", "ISO-8859-13");
        // registrar("ISO-8859-14", "ISO-8859-14");
        // registrar("iso-8859_14-1998", "ISO-8859-14");
        // registrar("iso-ir-199", "ISO-8859-14");
        // registrar("ISO_8859-14:1998", "ISO-8859-14");
        // registrar("latin8", "ISO-8859-14");
        // registrar("iso-celtic", "ISO-8859-14");
        // registrar("l8", "ISO-8859-14");
        // registrar("ISO-8859-15", "ISO-8859-15");
        // registrar("ibm-923_P100-1998", "ISO-8859-15");
        // registrar("ibm-923", "ISO-8859-15");
        // registrar("Latin-9", "ISO-8859-15");
        // registrar("l9", "ISO-8859-15");
        // registrar("8859_15", "ISO-8859-15");
        // registrar("latin0", "ISO-8859-15");
        // registrar("csisolatin0", "ISO-8859-15");
        // registrar("csisolatin9", "ISO-8859-15");
        // registrar("iso8859_15_fdis", "ISO-8859-15");
        // registrar("cp923", "ISO-8859-15");
        // registrar("923", "ISO-8859-15");
        // registrar("windows-28605", "ISO-8859-15");
        // registrar("Shift_JIS", "Shift_JIS");
        // registrar("ibm-943_P15A-2003", "Shift_JIS");
        // registrar("ibm-943", "Shift_JIS");
        // registrar("MS_Kanji", "Shift_JIS");
        // registrar("csShiftJIS", "Shift_JIS");
        // registrar("windows-31j", "Shift_JIS");
        // registrar("csWindows31J", "Shift_JIS");
        // registrar("x-sjis", "Shift_JIS");
        // registrar("x-ms-cp932", "Shift_JIS");
        // registrar("cp932", "Shift_JIS");
        // registrar("windows-932", "Shift_JIS");
        // registrar("cp943c", "Shift_JIS");
        // registrar("IBM-943C", "Shift_JIS");
        // registrar("ms932", "Shift_JIS");
        // registrar("pck", "Shift_JIS");
        // registrar("sjis", "Shift_JIS");
        // registrar("ibm-943_VSUB_VPUA", "Shift_JIS");
        // registrar("Extended_UNIX_Code_Packed_Format_for_Japanese", "Extended_UNIX_Code_Packed_Format_for_Japanese");
        // registrar("ibm-33722_P12A_P12A-2004_U2", "Extended_UNIX_Code_Packed_Format_for_Japanese");
        // registrar("ibm-33722", "Extended_UNIX_Code_Packed_Format_for_Japanese");
        // registrar("ibm-5050", "Extended_UNIX_Code_Packed_Format_for_Japanese");
        // registrar("EUC-JP", "Extended_UNIX_Code_Packed_Format_for_Japanese");
        // registrar("csEUCPkdFmtJapanese", "Extended_UNIX_Code_Packed_Format_for_Japanese");
        // registrar("X-EUC-JP", "Extended_UNIX_Code_Packed_Format_for_Japanese");
        // registrar("windows-51932", "Extended_UNIX_Code_Packed_Format_for_Japanese");
        // registrar("ibm-33722_VPUA", "Extended_UNIX_Code_Packed_Format_for_Japanese");
        // registrar("IBM-eucJP", "Extended_UNIX_Code_Packed_Format_for_Japanese");
        // registrar("Big5", "Big5");
        // registrar("windows-950-2000", "Big5");
        // registrar("csBig5", "Big5");
        // registrar("windows-950", "Big5");
        // registrar("x-big5", "Big5");
        // registrar("Big5-HKSCS", "Big5-HKSCS");
        // registrar("ibm-1375_P100-2007", "Big5-HKSCS");
        // registrar("ibm-1375", "Big5-HKSCS");
        // registrar("big5hk", "Big5-HKSCS");
        // registrar("HKSCS-BIG5", "Big5-HKSCS");
        // registrar("GBK", "GBK");
        // registrar("windows-936-2000", "GBK");
        // registrar("CP936", "GBK");
        // registrar("MS936", "GBK");
        // registrar("windows-936", "GBK");
        // registrar("GBK", "GBK");
        // registrar("ibm-1383_P110-1999", "GBK");
        // registrar("ibm-1383", "GBK");
        // registrar("GB2312", "GBK");
        // registrar("csGB2312", "GBK");
        // registrar("cp1383", "GBK");
        // registrar("1383", "GBK");
        // registrar("EUC-CN", "GBK");
        // registrar("ibm-eucCN", "GBK");
        // registrar("hp15CN", "GBK");
        // registrar("ibm-1383_VPUA", "GBK");
        // registrar("GBK", "GBK");
        // registrar("ibm-5478_P100-1995", "GBK");
        // registrar("ibm-5478", "GBK");
        // registrar("GB_2312-80", "GBK");
        // registrar("chinese", "GBK");
        // registrar("iso-ir-58", "GBK");
        // registrar("csISO58GB231280", "GBK");
        // registrar("gb2312-1980", "GBK");
        // registrar("GB2312.1980-0", "GBK");
        // registrar("windows-949-2000", "windows-949-2000");
        // registrar("ibm-970_P110_P110-2006_U2", "windows-949-2000");
        // registrar("ibm-970", "windows-949-2000");
        // registrar("EUC-KR", "windows-949-2000");
        // registrar("KS_C_5601-1987", "windows-949-2000");
        // registrar("windows-51949", "windows-949-2000");
        // registrar("csEUCKR", "windows-949-2000");
        // registrar("ibm-eucKR", "windows-949-2000");
        // registrar("KSC_5601", "windows-949-2000");
        // registrar("5601", "windows-949-2000");
        // registrar("cp970", "windows-949-2000");
        // registrar("970", "windows-949-2000");
        // registrar("ibm-970_VPUA", "windows-949-2000");
        // registrar("windows-949-2000", "windows-949-2000");
        // registrar("ibm-1363_P11B-1998", "windows-949-2000");
        // registrar("ibm-1363", "windows-949-2000");
        // registrar("KS_C_5601-1987", "windows-949-2000");
        // registrar("KS_C_5601-1989", "windows-949-2000");
        // registrar("KSC_5601", "windows-949-2000");
        // registrar("csKSC56011987", "windows-949-2000");
        // registrar("korean", "windows-949-2000");
        // registrar("iso-ir-149", "windows-949-2000");
        // registrar("5601", "windows-949-2000");
        // registrar("cp1363", "windows-949-2000");
        // registrar("ksc", "windows-949-2000");
        // registrar("windows-949", "windows-949-2000");
        // registrar("ibm-1363_VSUB_VPUA", "windows-949-2000");
        // registrar("windows-874-2000", "windows-874-2000");
        // registrar("ibm-874_P100-1995", "windows-874-2000");
        // registrar("ibm-874", "windows-874-2000");
        // registrar("ibm-9066", "windows-874-2000");
        // registrar("cp874", "windows-874-2000");
        // registrar("TIS-620", "windows-874-2000");
        // registrar("tis620.2533", "windows-874-2000");
        // registrar("eucTH", "windows-874-2000");
        // registrar("IBM437", "IBM437");
        // registrar("ibm-437_P100-1995", "IBM437");
        // registrar("ibm-437", "IBM437");
        // registrar("cp437", "IBM437");
        // registrar("437", "IBM437");
        // registrar("csPC8CodePage437", "IBM437");
        // registrar("windows-437", "IBM437");
        // registrar("IBM775", "IBM775");
        // registrar("ibm-775_P100-1996", "IBM775");
        // registrar("ibm-775", "IBM775");
        // registrar("cp775", "IBM775");
        // registrar("csPC775Baltic", "IBM775");
        // registrar("windows-775", "IBM775");
        // registrar("775", "IBM775");
        // registrar("IBM850", "IBM850");
        // registrar("ibm-850_P100-1995", "IBM850");
        // registrar("ibm-850", "IBM850");
        // registrar("cp850", "IBM850");
        // registrar("850", "IBM850");
        // registrar("csPC850Multilingual", "IBM850");
        // registrar("windows-850", "IBM850");
        // registrar("IBM851", "IBM851");
        // registrar("ibm-851_P100-1995", "IBM851");
        // registrar("ibm-851", "IBM851");
        // registrar("cp851", "IBM851");
        // registrar("851", "IBM851");
        // registrar("csPC851", "IBM851");
        // registrar("IBM852", "IBM852");
        // registrar("ibm-852_P100-1995", "IBM852");
        // registrar("ibm-852", "IBM852");
        // registrar("cp852", "IBM852");
        // registrar("852", "IBM852");
        // registrar("csPCp852", "IBM852");
        // registrar("windows-852", "IBM852");
        // registrar("IBM855", "IBM855");
        // registrar("ibm-855_P100-1995", "IBM855");
        // registrar("ibm-855", "IBM855");
        // registrar("cp855", "IBM855");
        // registrar("855", "IBM855");
        // registrar("csIBM855", "IBM855");
        // registrar("csPCp855", "IBM855");
        // registrar("windows-855", "IBM855");
        // registrar("IBM857", "IBM857");
        // registrar("ibm-857_P100-1995", "IBM857");
        // registrar("ibm-857", "IBM857");
        // registrar("cp857", "IBM857");
        // registrar("857", "IBM857");
        // registrar("csIBM857", "IBM857");
        // registrar("windows-857", "IBM857");
        // registrar("IBM00858", "IBM00858");
        // registrar("ibm-858_P100-1997", "IBM00858");
        // registrar("ibm-858", "IBM00858");
        // registrar("CCSID00858", "IBM00858");
        // registrar("CP00858", "IBM00858");
        // registrar("PC-Multilingual-850+euro", "IBM00858");
        // registrar("cp858", "IBM00858");
        // registrar("windows-858", "IBM00858");
        // registrar("IBM860", "IBM860");
        // registrar("ibm-860_P100-1995", "IBM860");
        // registrar("ibm-860", "IBM860");
        // registrar("cp860", "IBM860");
        // registrar("860", "IBM860");
        // registrar("csIBM860", "IBM860");
        // registrar("IBM861", "IBM861");
        // registrar("ibm-861_P100-1995", "IBM861");
        // registrar("ibm-861", "IBM861");
        // registrar("cp861", "IBM861");
        // registrar("861", "IBM861");
        // registrar("cp-is", "IBM861");
        // registrar("csIBM861", "IBM861");
        // registrar("windows-861", "IBM861");
        // registrar("IBM862", "IBM862");
        // registrar("ibm-862_P100-1995", "IBM862");
        // registrar("ibm-862", "IBM862");
        // registrar("cp862", "IBM862");
        // registrar("862", "IBM862");
        // registrar("csPC862LatinHebrew", "IBM862");
        // registrar("DOS-862", "IBM862");
        // registrar("windows-862", "IBM862");
        // registrar("IBM863", "IBM863");
        // registrar("ibm-863_P100-1995", "IBM863");
        // registrar("ibm-863", "IBM863");
        // registrar("cp863", "IBM863");
        // registrar("863", "IBM863");
        // registrar("csIBM863", "IBM863");
        // registrar("IBM864", "IBM864");
        // registrar("ibm-864_X110-1999", "IBM864");
        // registrar("ibm-864", "IBM864");
        // registrar("cp864", "IBM864");
        // registrar("csIBM864", "IBM864");
        // registrar("IBM865", "IBM865");
        // registrar("ibm-865_P100-1995", "IBM865");
        // registrar("ibm-865", "IBM865");
        // registrar("cp865", "IBM865");
        // registrar("865", "IBM865");
        // registrar("csIBM865", "IBM865");
        // registrar("IBM866", "IBM866");
        // registrar("ibm-866_P100-1995", "IBM866");
        // registrar("ibm-866", "IBM866");
        // registrar("cp866", "IBM866");
        // registrar("866", "IBM866");
        // registrar("csIBM866", "IBM866");
        // registrar("windows-866", "IBM866");
        // registrar("IBM868", "IBM868");
        // registrar("ibm-868_P100-1995", "IBM868");
        // registrar("ibm-868", "IBM868");
        // registrar("CP868", "IBM868");
        // registrar("868", "IBM868");
        // registrar("csIBM868", "IBM868");
        // registrar("cp-ar", "IBM868");
        // registrar("IBM869", "IBM869");
        // registrar("ibm-869_P100-1995", "IBM869");
        // registrar("ibm-869", "IBM869");
        // registrar("cp869", "IBM869");
        // registrar("869", "IBM869");
        // registrar("cp-gr", "IBM869");
        // registrar("csIBM869", "IBM869");
        // registrar("windows-869", "IBM869");
        // registrar("KOI8-R", "KOI8-R");
        // registrar("ibm-878_P100-1996", "KOI8-R");
        // registrar("ibm-878", "KOI8-R");
        // registrar("koi8", "KOI8-R");
        // registrar("csKOI8R", "KOI8-R");
        // registrar("windows-20866", "KOI8-R");
        // registrar("cp878", "KOI8-R");
        // registrar("KOI8-U", "KOI8-U");
        // registrar("ibm-1168_P100-2002", "KOI8-U");
        // registrar("ibm-1168", "KOI8-U");
        // registrar("windows-21866", "KOI8-U");
        // registrar("windows-1250", "windows-1250");
        // registrar("ibm-5346_P100-1998", "windows-1250");
        // registrar("ibm-5346", "windows-1250");
        // registrar("cp1250", "windows-1250");
        // registrar("windows-1251", "windows-1251");
        // registrar("ibm-5347_P100-1998", "windows-1251");
        // registrar("ibm-5347", "windows-1251");
        // registrar("cp1251", "windows-1251");
        // registrar("ANSI1251", "windows-1251");
        // registrar("windows-1252", "windows-1252");
        // registrar("ibm-5348_P100-1997", "windows-1252");
        // registrar("ibm-5348", "windows-1252");
        // registrar("cp1252", "windows-1252");
        // registrar("windows-1253", "windows-1253");
        // registrar("ibm-5349_P100-1998", "windows-1253");
        // registrar("ibm-5349", "windows-1253");
        // registrar("cp1253", "windows-1253");
        // registrar("windows-1254", "windows-1254");
        // registrar("ibm-5350_P100-1998", "windows-1254");
        // registrar("ibm-5350", "windows-1254");
        // registrar("cp1254", "windows-1254");
        // registrar("windows-1255", "windows-1255");
        // registrar("ibm-9447_P100-2002", "windows-1255");
        // registrar("ibm-9447", "windows-1255");
        // registrar("cp1255", "windows-1255");
        // registrar("windows-1256", "windows-1256");
        // registrar("ibm-9448_X100-2005", "windows-1256");
        // registrar("ibm-9448", "windows-1256");
        // registrar("cp1256", "windows-1256");
        // registrar("windows-1257", "windows-1257");
        // registrar("ibm-9449_P100-2002", "windows-1257");
        // registrar("ibm-9449", "windows-1257");
        // registrar("cp1257", "windows-1257");
        // registrar("windows-1258", "windows-1258");
        // registrar("ibm-5354_P100-1998", "windows-1258");
        // registrar("ibm-5354", "windows-1258");
        // registrar("cp1258", "windows-1258");
        // registrar("macintosh", "macintosh");
        // registrar("macos-0_2-10.2", "macintosh");
        // registrar("mac", "macintosh");
        // registrar("csMacintosh", "macintosh");
        // registrar("windows-10000", "macintosh");
        // registrar("hp-roman8", "hp-roman8");
        // registrar("ibm-1051_P100-1995", "hp-roman8");
        // registrar("ibm-1051", "hp-roman8");
        // registrar("roman8", "hp-roman8");
        // registrar("r8", "hp-roman8");
        // registrar("csHPRoman8", "hp-roman8");
        // registrar("Adobe-Standard-Encoding", "Adobe-Standard-Encoding");
        // registrar("ibm-1276_P100-1995", "Adobe-Standard-Encoding");
        // registrar("ibm-1276", "Adobe-Standard-Encoding");
        // registrar("csAdobeStandardEncoding", "Adobe-Standard-Encoding");
        // registrar("ISO-2022-JP", "ISO-2022-JP");
        // registrar("ISO_2022,locale=ja,version=0", "ISO-2022-JP");
        // registrar("csISO2022JP", "ISO-2022-JP");
        // registrar("JIS_Encoding", "JIS_Encoding");
        // registrar("ISO_2022,locale=ja,version=1", "JIS_Encoding");
        // registrar("ISO-2022-JP-1", "JIS_Encoding");
        // registrar("csJISEncoding", "JIS_Encoding");
        // registrar("ibm-5054", "JIS_Encoding");
        // registrar("JIS", "JIS_Encoding");
        // registrar("ISO-2022-JP-2", "ISO-2022-JP-2");
        // registrar("ISO_2022,locale=ja,version=2", "ISO-2022-JP-2");
        // registrar("csISO2022JP2", "ISO-2022-JP-2");
        // registrar("ISO-2022-KR", "ISO-2022-KR");
        // registrar("ISO_2022,locale=ko,version=0", "ISO-2022-KR");
        // registrar("csISO2022KR", "ISO-2022-KR");
        // registrar("ISO-2022-CN", "ISO-2022-CN");
        // registrar("ISO_2022,locale=zh,version=0", "ISO-2022-CN");
        // registrar("csISO2022CN", "ISO-2022-CN");
        // registrar("ISO-2022-CN-EXT", "ISO-2022-CN-EXT");
        // registrar("ISO_2022,locale=zh,version=1", "ISO-2022-CN-EXT");
        // registrar("HZ-GB-2312", "HZ-GB-2312");
        // registrar("HZ", "HZ-GB-2312");
        // registrar("JIS_X0201", "JIS_X0201");
        // registrar("ibm-897_P100-1995", "JIS_X0201");
        // registrar("ibm-897", "JIS_X0201");
        // registrar("X0201", "JIS_X0201");
        // registrar("csHalfWidthKatakana", "JIS_X0201");
        // registrar("IBM037", "IBM037");
        // registrar("ibm-37_P100-1995", "IBM037");
        // registrar("ibm-37", "IBM037");
        // registrar("ibm-037", "IBM037");
        // registrar("ebcdic-cp-us", "IBM037");
        // registrar("ebcdic-cp-ca", "IBM037");
        // registrar("ebcdic-cp-wt", "IBM037");
        // registrar("ebcdic-cp-nl", "IBM037");
        // registrar("csIBM037", "IBM037");
        // registrar("cp037", "IBM037");
        // registrar("037", "IBM037");
        // registrar("cpibm37", "IBM037");
        // registrar("cp37", "IBM037");
        // registrar("IBM273", "IBM273");
        // registrar("ibm-273_P100-1995", "IBM273");
        // registrar("ibm-273", "IBM273");
        // registrar("CP273", "IBM273");
        // registrar("csIBM273", "IBM273");
        // registrar("ebcdic-de", "IBM273");
        // registrar("273", "IBM273");
        // registrar("IBM277", "IBM277");
        // registrar("ibm-277_P100-1995", "IBM277");
        // registrar("ibm-277", "IBM277");
        // registrar("cp277", "IBM277");
        // registrar("EBCDIC-CP-DK", "IBM277");
        // registrar("EBCDIC-CP-NO", "IBM277");
        // registrar("csIBM277", "IBM277");
        // registrar("ebcdic-dk", "IBM277");
        // registrar("277", "IBM277");
        // registrar("IBM278", "IBM278");
        // registrar("ibm-278_P100-1995", "IBM278");
        // registrar("ibm-278", "IBM278");
        // registrar("cp278", "IBM278");
        // registrar("ebcdic-cp-fi", "IBM278");
        // registrar("ebcdic-cp-se", "IBM278");
        // registrar("csIBM278", "IBM278");
        // registrar("ebcdic-sv", "IBM278");
        // registrar("278", "IBM278");
        // registrar("IBM280", "IBM280");
        // registrar("ibm-280_P100-1995", "IBM280");
        // registrar("ibm-280", "IBM280");
        // registrar("CP280", "IBM280");
        // registrar("ebcdic-cp-it", "IBM280");
        // registrar("csIBM280", "IBM280");
        // registrar("280", "IBM280");
        // registrar("IBM284", "IBM284");
        // registrar("ibm-284_P100-1995", "IBM284");
        // registrar("ibm-284", "IBM284");
        // registrar("CP284", "IBM284");
        // registrar("ebcdic-cp-es", "IBM284");
        // registrar("csIBM284", "IBM284");
        // registrar("cpibm284", "IBM284");
        // registrar("284", "IBM284");
        // registrar("IBM285", "IBM285");
        // registrar("ibm-285_P100-1995", "IBM285");
        // registrar("ibm-285", "IBM285");
        // registrar("CP285", "IBM285");
        // registrar("ebcdic-cp-gb", "IBM285");
        // registrar("csIBM285", "IBM285");
        // registrar("cpibm285", "IBM285");
        // registrar("ebcdic-gb", "IBM285");
        // registrar("285", "IBM285");
        // registrar("IBM290", "IBM290");
        // registrar("ibm-290_P100-1995", "IBM290");
        // registrar("ibm-290", "IBM290");
        // registrar("cp290", "IBM290");
        // registrar("EBCDIC-JP-kana", "IBM290");
        // registrar("csIBM290", "IBM290");
        // registrar("IBM297", "IBM297");
        // registrar("ibm-297_P100-1995", "IBM297");
        // registrar("ibm-297", "IBM297");
        // registrar("cp297", "IBM297");
        // registrar("ebcdic-cp-fr", "IBM297");
        // registrar("csIBM297", "IBM297");
        // registrar("cpibm297", "IBM297");
        // registrar("297", "IBM297");
        // registrar("IBM420", "IBM420");
        // registrar("ibm-420_X120-1999", "IBM420");
        // registrar("ibm-420", "IBM420");
        // registrar("cp420", "IBM420");
        // registrar("ebcdic-cp-ar1", "IBM420");
        // registrar("csIBM420", "IBM420");
        // registrar("420", "IBM420");
        // registrar("IBM424", "IBM424");
        // registrar("ibm-424_P100-1995", "IBM424");
        // registrar("ibm-424", "IBM424");
        // registrar("cp424", "IBM424");
        // registrar("ebcdic-cp-he", "IBM424");
        // registrar("csIBM424", "IBM424");
        // registrar("424", "IBM424");
        // registrar("IBM500", "IBM500");
        // registrar("ibm-500_P100-1995", "IBM500");
        // registrar("ibm-500", "IBM500");
        // registrar("CP500", "IBM500");
        // registrar("ebcdic-cp-be", "IBM500");
        // registrar("csIBM500", "IBM500");
        // registrar("ebcdic-cp-ch", "IBM500");
        // registrar("500", "IBM500");
        // registrar("IBM-Thai", "IBM-Thai");
        // registrar("ibm-838_P100-1995", "IBM-Thai");
        // registrar("ibm-838", "IBM-Thai");
        // registrar("IBM838", "IBM-Thai");
        // registrar("csIBMThai", "IBM-Thai");
        // registrar("cp838", "IBM-Thai");
        // registrar("838", "IBM-Thai");
        // registrar("ibm-9030", "IBM-Thai");
        // registrar("IBM870", "IBM870");
        // registrar("ibm-870_P100-1995", "IBM870");
        // registrar("ibm-870", "IBM870");
        // registrar("CP870", "IBM870");
        // registrar("ebcdic-cp-roece", "IBM870");
        // registrar("ebcdic-cp-yu", "IBM870");
        // registrar("csIBM870", "IBM870");
        // registrar("IBM871", "IBM871");
        // registrar("ibm-871_P100-1995", "IBM871");
        // registrar("ibm-871", "IBM871");
        // registrar("ebcdic-cp-is", "IBM871");
        // registrar("csIBM871", "IBM871");
        // registrar("CP871", "IBM871");
        // registrar("ebcdic-is", "IBM871");
        // registrar("871", "IBM871");
        // registrar("IBM918", "IBM918");
        // registrar("ibm-918_P100-1995", "IBM918");
        // registrar("ibm-918", "IBM918");
        // registrar("CP918", "IBM918");
        // registrar("ebcdic-cp-ar2", "IBM918");
        // registrar("csIBM918", "IBM918");
        // registrar("IBM1026", "IBM1026");
        // registrar("ibm-1026_P100-1995", "IBM1026");
        // registrar("ibm-1026", "IBM1026");
        // registrar("CP1026", "IBM1026");
        // registrar("csIBM1026", "IBM1026");
        // registrar("1026", "IBM1026");
        // registrar("IBM1047", "IBM1047");
        // registrar("ibm-1047_P100-1995", "IBM1047");
        // registrar("ibm-1047", "IBM1047");
        // registrar("cp1047", "IBM1047");
        // registrar("1047", "IBM1047");
        // registrar("IBM01140", "IBM01140");
        // registrar("ibm-1140_P100-1997", "IBM01140");
        // registrar("ibm-1140", "IBM01140");
        // registrar("CCSID01140", "IBM01140");
        // registrar("CP01140", "IBM01140");
        // registrar("cp1140", "IBM01140");
        // registrar("ebcdic-us-37+euro", "IBM01140");
        // registrar("IBM01141", "IBM01141");
        // registrar("ibm-1141_P100-1997", "IBM01141");
        // registrar("ibm-1141", "IBM01141");
        // registrar("CCSID01141", "IBM01141");
        // registrar("CP01141", "IBM01141");
        // registrar("cp1141", "IBM01141");
        // registrar("ebcdic-de-273+euro", "IBM01141");
        // registrar("IBM01142", "IBM01142");
        // registrar("ibm-1142_P100-1997", "IBM01142");
        // registrar("ibm-1142", "IBM01142");
        // registrar("CCSID01142", "IBM01142");
        // registrar("CP01142", "IBM01142");
        // registrar("cp1142", "IBM01142");
        // registrar("ebcdic-dk-277+euro", "IBM01142");
        // registrar("ebcdic-no-277+euro", "IBM01142");
        // registrar("IBM01143", "IBM01143");
        // registrar("ibm-1143_P100-1997", "IBM01143");
        // registrar("ibm-1143", "IBM01143");
        // registrar("CCSID01143", "IBM01143");
        // registrar("CP01143", "IBM01143");
        // registrar("cp1143", "IBM01143");
        // registrar("ebcdic-fi-278+euro", "IBM01143");
        // registrar("ebcdic-se-278+euro", "IBM01143");
        // registrar("IBM01144", "IBM01144");
        // registrar("ibm-1144_P100-1997", "IBM01144");
        // registrar("ibm-1144", "IBM01144");
        // registrar("CCSID01144", "IBM01144");
        // registrar("CP01144", "IBM01144");
        // registrar("cp1144", "IBM01144");
        // registrar("ebcdic-it-280+euro", "IBM01144");
        // registrar("IBM01145", "IBM01145");
        // registrar("ibm-1145_P100-1997", "IBM01145");
        // registrar("ibm-1145", "IBM01145");
        // registrar("CCSID01145", "IBM01145");
        // registrar("CP01145", "IBM01145");
        // registrar("cp1145", "IBM01145");
        // registrar("ebcdic-es-284+euro", "IBM01145");
        // registrar("IBM01146", "IBM01146");
        // registrar("ibm-1146_P100-1997", "IBM01146");
        // registrar("ibm-1146", "IBM01146");
        // registrar("CCSID01146", "IBM01146");
        // registrar("CP01146", "IBM01146");
        // registrar("cp1146", "IBM01146");
        // registrar("ebcdic-gb-285+euro", "IBM01146");
        // registrar("IBM01147", "IBM01147");
        // registrar("ibm-1147_P100-1997", "IBM01147");
        // registrar("ibm-1147", "IBM01147");
        // registrar("CCSID01147", "IBM01147");
        // registrar("CP01147", "IBM01147");
        // registrar("cp1147", "IBM01147");
        // registrar("ebcdic-fr-297+euro", "IBM01147");
        // registrar("IBM01148", "IBM01148");
        // registrar("ibm-1148_P100-1997", "IBM01148");
        // registrar("ibm-1148", "IBM01148");
        // registrar("CCSID01148", "IBM01148");
        // registrar("CP01148", "IBM01148");
        // registrar("cp1148", "IBM01148");
        // registrar("ebcdic-international-500+euro", "IBM01148");
        // registrar("IBM01149", "IBM01149");
        // registrar("ibm-1149_P100-1997", "IBM01149");
        // registrar("ibm-1149", "IBM01149");
        // registrar("CCSID01149", "IBM01149");
        // registrar("CP01149", "IBM01149");
        // registrar("cp1149", "IBM01149");
        // registrar("ebcdic-is-871+euro", "IBM01149");

        // Additional aliases.
        // Perhaps we can get these added to ICU.
        // registrar("macroman", "macintosh");
        // registrar("xmacroman", "macintosh");

        // Additional aliases that historically were present in the encoding
        // table in WebKit on Macintosh that don't seem to be present in ICU.
        // Perhaps we can prove these are not used on the web and remove them.
        // Or perhaps we can get them added to ICU.
        // registrar("cnbig5", "Big5");
        // registrar("cngb", "EUC-CN");
        // registrar("csISO88598I", "ISO_8859-8-I");
        // registrar("csgb231280", "EUC-CN");
        // registrar("dos720", "cp864");
        // registrar("dos874", "cp874");
        // registrar("jis7", "ISO-2022-JP");
        // registrar("koi", "KOI8-R");
        // registrar("logical", "ISO-8859-8-I");
        // registrar("unicode11utf8", "UTF-8");
        // registrar("unicode20utf8", "UTF-8");
        // registrar("visual", "ISO-8859-8");
        // registrar("winarabic", "windows-1256");
        // registrar("winbaltic", "windows-1257");
        // registrar("wincyrillic", "windows-1251");
        // registrar("windows874", "windows874-2000");
        // registrar("iso885911", "windows874-2000");
        // registrar("wingreek", "windows-1253");
        // registrar("winhebrew", "windows-1255");
        // registrar("winlatin2", "windows-1250");
        // registrar("winturkish", "windows-1254");
        // registrar("winvietnamese", "windows-1258");
        // registrar("xcp1250", "windows-1250");
        // registrar("xcp1251", "windows-1251");
        // registrar("xeuc", "EUC-JP");
        // registrar("xeuccn", "EUC-CN");
        // registrar("xgbk", "EUC-CN");
        // registrar("xunicode20utf8", "UTF-8");
        // registrar("xxbig5", "Big5");

}

void TextCodecICU::registerExtendedCodecs(TextCodecRegistrar registrar)
{
    // See comment above in registerEncodingNames.
    //registrar("ISO-8859-8-I", newTextCodecICU, 0);
    registrar("UTF-8", newTextCodecICU, 0);
    //registrar("UTF-16", newTextCodecICU, 0);
    //registrar("UTF-16BE", newTextCodecICU, 0);
    //registrar("UTF-16LE", newTextCodecICU, 0);
        // registrar("UTF-32", newTextCodecICU, 0);
        // registrar("UTF-32BE", newTextCodecICU, 0);
        // registrar("UTF-32LE", newTextCodecICU, 0);
        // registrar("UTF-7", newTextCodecICU, 0);
        // registrar("SCSU", newTextCodecICU, 0);
        // registrar("BOCU-1", newTextCodecICU, 0);
        // registrar("CESU-8", newTextCodecICU, 0);
        // registrar("ISO_8859-1:1987", newTextCodecICU, 0);
        // registrar("ANSI_X3.4-1968", newTextCodecICU, 0);
        // registrar("gb18030", newTextCodecICU, 0);
        // registrar("ISO_8859-2:1987", newTextCodecICU, 0);
        // registrar("ISO_8859-3:1988", newTextCodecICU, 0);
        // registrar("ISO_8859-4:1988", newTextCodecICU, 0);
        // registrar("ISO_8859-5:1988", newTextCodecICU, 0);
        // registrar("ISO_8859-6:1987", newTextCodecICU, 0);
        // registrar("ISO_8859-7:1987", newTextCodecICU, 0);
        // registrar("ISO_8859-8:1988", newTextCodecICU, 0);
        // registrar("ISO_8859-9:1989", newTextCodecICU, 0);
        // registrar("ISO-8859-10", newTextCodecICU, 0);
        // registrar("ISO-8859-13", newTextCodecICU, 0);
        // registrar("ISO-8859-14", newTextCodecICU, 0);
        // registrar("ISO-8859-15", newTextCodecICU, 0);
        // registrar("Shift_JIS", newTextCodecICU, 0);
        // registrar("Extended_UNIX_Code_Packed_Format_for_Japanese", newTextCodecICU, 0);
        // registrar("Big5", newTextCodecICU, 0);
        // registrar("Big5-HKSCS", newTextCodecICU, 0);
        // registrar("GBK", newTextCodecICU, 0);
        // registrar("GB2312", newTextCodecICU, 0);
        // registrar("GB_2312-80", newTextCodecICU, 0);
        // registrar("EUC-KR", newTextCodecICU, 0);
        // registrar("KS_C_5601-1987", newTextCodecICU, 0);
        // registrar("TIS-620", newTextCodecICU, 0);
        // registrar("IBM437", newTextCodecICU, 0);
        // registrar("IBM775", newTextCodecICU, 0);
        // registrar("IBM850", newTextCodecICU, 0);
        // registrar("IBM851", newTextCodecICU, 0);
        // registrar("IBM852", newTextCodecICU, 0);
        // registrar("IBM855", newTextCodecICU, 0);
        // registrar("IBM857", newTextCodecICU, 0);
        // registrar("IBM00858", newTextCodecICU, 0);
        // registrar("IBM860", newTextCodecICU, 0);
        // registrar("IBM861", newTextCodecICU, 0);
        // registrar("IBM862", newTextCodecICU, 0);
        // registrar("IBM863", newTextCodecICU, 0);
        // registrar("IBM864", newTextCodecICU, 0);
        // registrar("IBM865", newTextCodecICU, 0);
        // registrar("IBM866", newTextCodecICU, 0);
        // registrar("IBM868", newTextCodecICU, 0);
        // registrar("IBM869", newTextCodecICU, 0);
        // registrar("KOI8-R", newTextCodecICU, 0);
        // registrar("KOI8-U", newTextCodecICU, 0);
        // registrar("windows-1250", newTextCodecICU, 0);
        // registrar("windows-1251", newTextCodecICU, 0);
        // registrar("windows-1252", newTextCodecICU, 0);
        // registrar("windows-1253", newTextCodecICU, 0);
        // registrar("windows-1254", newTextCodecICU, 0);
        // registrar("windows-1255", newTextCodecICU, 0);
        // registrar("windows-1256", newTextCodecICU, 0);
        // registrar("windows-1257", newTextCodecICU, 0);
        // registrar("windows-1258", newTextCodecICU, 0);
        // registrar("macintosh", newTextCodecICU, 0);
        // registrar("hp-roman8", newTextCodecICU, 0);
        // registrar("Adobe-Standard-Encoding", newTextCodecICU, 0);
        // registrar("ISO-2022-JP", newTextCodecICU, 0);
        // registrar("JIS_Encoding", newTextCodecICU, 0);
        // registrar("ISO-2022-JP-2", newTextCodecICU, 0);
        // registrar("ISO-2022-KR", newTextCodecICU, 0);
        // registrar("ISO-2022-CN", newTextCodecICU, 0);
        // registrar("ISO-2022-CN-EXT", newTextCodecICU, 0);
        // registrar("HZ-GB-2312", newTextCodecICU, 0);
        // registrar("JIS_X0201", newTextCodecICU, 0);
        // registrar("IBM037", newTextCodecICU, 0);
        // registrar("IBM273", newTextCodecICU, 0);
        // registrar("IBM277", newTextCodecICU, 0);
        // registrar("IBM278", newTextCodecICU, 0);
        // registrar("IBM280", newTextCodecICU, 0);
        // registrar("IBM284", newTextCodecICU, 0);
        // registrar("IBM285", newTextCodecICU, 0);
        // registrar("IBM290", newTextCodecICU, 0);
        // registrar("IBM297", newTextCodecICU, 0);
        // registrar("IBM420", newTextCodecICU, 0);
        // registrar("IBM424", newTextCodecICU, 0);
        // registrar("IBM500", newTextCodecICU, 0);
        // registrar("IBM-Thai", newTextCodecICU, 0);
        // registrar("IBM870", newTextCodecICU, 0);
        // registrar("IBM871", newTextCodecICU, 0);
        // registrar("IBM918", newTextCodecICU, 0);
        // registrar("IBM1026", newTextCodecICU, 0);
        // registrar("IBM1047", newTextCodecICU, 0);
        // registrar("IBM01140", newTextCodecICU, 0);
        // registrar("IBM01141", newTextCodecICU, 0);
        // registrar("IBM01142", newTextCodecICU, 0);
        // registrar("IBM01143", newTextCodecICU, 0);
        // registrar("IBM01144", newTextCodecICU, 0);
        // registrar("IBM01145", newTextCodecICU, 0);
        // registrar("IBM01146", newTextCodecICU, 0);
        // registrar("IBM01147", newTextCodecICU, 0);
        // registrar("IBM01148", newTextCodecICU, 0);
        // registrar("IBM01149", newTextCodecICU, 0);
}

TextCodecICU::TextCodecICU(const TextEncoding& encoding)
    : m_encoding(encoding)
    , m_numBufferedBytes(0)
	, m_expectedSize(0)
{
    #if _DEBUG
        memset(m_bufferedBytes, 0, sizeof(m_bufferedBytes));
    #endif
}


// This function can be called multiple times successively, with flush being false for
// each time but the last. It's possible that the data may be such that a multibyte
// character straddles the division between two blocks given in successive calls.
// So we must save a little buffer of m_bufferedBytes to deal with this.
// In our implementation here, we efficiently solve UTF-8 decoding without allocating
// memory in most cases, including the case whereby we recieve incomplete multi-byte
// sequences.

//Note by Arpit Baldeva: Use the string buffer for decoding instead of the conditional stack based. This is simply because it did not save us any allocation.
//After decoding, we end up appending to a String which does the allocation we avoided. Worst case, we could be decoding a 90K js/css in 1 shot(with the
//cached script/css mechanism) and end up allocating 180K (90K * sizeof(UChar))buffer twice. This is because we free the decode buffer "after" appending the data
//to the string we return. So our peak memory usage would be doubled.
//Coincidentally, after doing this change, I realized that UTF16 text codec also uses a StringBuffer.
#define USE_STRING_BUFFER_FOR_DECODING 1

String TextCodecICU::decode(const char* bytes, size_t length, bool flush, bool /*stopOnError*/, bool& sawError)
{
    String resultString;

    // To consider: support 'sawError'.
    sawError = false;

    // ASSERT(m_encoding == "UTF-8");

    if(length)
    {
        if(m_numBufferedBytes) // If we have any leftover bytes from the last call...
        {
            const size_t additionalRequiredBytes = (m_expectedSize - m_numBufferedBytes);

            if(length >= additionalRequiredBytes) // If we now have enough bytes to finish the current UTF8 char... (in the large majority of cases this expression will be true, as additionalRequiredBytes is usually only 1 or 2)
            {
                // We build up a little multi-byte sequence for just this char and call ourselves recursively.
                char tempBuffer[kBufferSize];

                memcpy(tempBuffer, m_bufferedBytes, m_numBufferedBytes);
                memcpy(tempBuffer + m_numBufferedBytes, bytes, additionalRequiredBytes);

                m_numBufferedBytes = 0;
                bytes  += additionalRequiredBytes;
                length -= additionalRequiredBytes;

                // This should always result in exactly one decoded char with no leftover bytes.
                resultString = decode(tempBuffer, m_expectedSize, false, false, sawError);
                ASSERT((resultString.length() == 1) && (m_numBufferedBytes == 0));
            }
            else if((m_numBufferedBytes + length) < (size_t)kBufferSize)  // If we haven't gone over our buffer... (this expression should always be true)
            {
                // This pathway can execute if we have, for example, 1 buffered byte, 
                // are expecting 2 more, but length is only 1. This is unlikely but 
                // within the specifications of this decode API.
                memcpy(m_bufferedBytes + m_numBufferedBytes, bytes, length);
                m_numBufferedBytes += length;
				length = 0;//+ Note by Arpit Baldeva - We have buffered everything.
            }
            else // Else our buffer was not large enough to hold the multi-byte sequence. Currently this should not be possible based on our logic below.
            {
                m_numBufferedBytes = 0; // We panic and boot the buffer.
            }
        }
#if USE_STRING_BUFFER_FOR_DECODING
		//This is possible due to recursive nature of this function. Fortunately, we would only see 1 level of recursion.
		int existingLength = resultString.length();
		StringBuffer resultBuffer(length+existingLength);
		if(existingLength)
		{
			memcpy(resultBuffer.characters(),resultString.characters(),resultString.length()*sizeof(UChar));//copy the decoded string
		}
		UChar* pUsedBuffer = resultBuffer.characters() + existingLength; //offset the buffer
#else
        // We use stack space for the encoding where possible.
        #if defined(_WIN32) || defined(EA_PLATFORM_UNIX)
             const size_t kMaxStackSize = 4096;
        #else
             const size_t kMaxStackSize = 1024;
        #endif
        UChar*       pMallocBuffer = NULL;
        UChar*       pUsedBuffer   = ((length * sizeof(UChar)) < kMaxStackSize) ? (UChar*)alloca(length * sizeof(UChar)) : (pMallocBuffer = (UChar*)fastMalloc(length * sizeof(UChar)));
#endif 
		size_t       j = 0;

        for(size_t i = 0; i<length; ++i, ++j)
        {
            if (((unsigned char)bytes[i] & 240) == 240)  // If we appear to have a 4-byte UTF8 sequence...
            {
                // We need to validate if (i + n) is within length and if not then copy it to m_bufferedBytes.
                if((i + 3) < length)  // If the expected length is within the actual length...
                {
                    if(((unsigned char)bytes[i+1] & 128) && ((unsigned char)bytes[i+2] & 128) && ((unsigned char)bytes[i+3] & 128))
                        pUsedBuffer[j] = ((unsigned char)bytes[i] - 240) * 65536 + ((unsigned char)bytes[i+1] - 128) * 4096 + ((unsigned char)bytes[i+2] - 128) * 64 + ((unsigned char)bytes[i+3] - 128);
                    else
                    {
                        sawError = true;
                        flush    = true;
                    }
                }
                else  // Else the actual length isn't long enough.
                {
                    m_expectedSize = 4;
                    m_numBufferedBytes += (length - i);
                    memcpy(m_bufferedBytes, bytes + i, length - i);
                    break;
                }

                i += 3;
            }
            else if ( ((unsigned char)bytes[i]   & 224) == 224 )
            {
                if((i + 2) < length)
                    if(((unsigned char)bytes[i+1] & 128) && ((unsigned char)bytes[i+2] & 128))
                        pUsedBuffer[j] = ((unsigned char)bytes[i] - 224) * 4096 + ((unsigned char)bytes[i+1] - 128) * 64 + ((unsigned char)bytes[i+2] - 128);
                    else
                    {
                        sawError = true;
                        flush    = true;
                    }
                else
                {
                    m_expectedSize = 3;
                    m_numBufferedBytes += (length - i);
                    memcpy(m_bufferedBytes, bytes + i, length - i);
                    break;
                }

                i += 2;
            }
            else if ( ((unsigned char)bytes[i]   & 192) == 192) 
            {
                if((i + 1) < length)
                {
                    if((unsigned char)bytes[i+1] & 128)
                        pUsedBuffer[j] = ((unsigned char)bytes[i] - 192) * 64 + ((unsigned char)bytes[i+1] - 128);
                    else
                    {
                        sawError = true;
                        flush    = true;
                    }
                }
                else
                {
                    m_expectedSize = 2;
                    m_bufferedBytes[m_numBufferedBytes++] = bytes[i];
                    break;
                }

                i++;
            }
            else if (((unsigned char)bytes[i] & 128) == 128)
                pUsedBuffer[j] = (unsigned char)bytes[i] & 127;
            else
                pUsedBuffer[j] = (unsigned char)bytes[i];
        }
#if USE_STRING_BUFFER_FOR_DECODING
		if(flush) // If flush is true, then this is the last call, so we unilaterally clear our buffer.
			m_numBufferedBytes = 0;

		resultBuffer.resize(j+existingLength);//Make sure we don't overwrite the leftover. Resize is pretty efficient as it simply adjusts length when going down in size as is the case here.
		return String::adopt(resultBuffer);
#else		
		resultString.append(pUsedBuffer, j);
		if(pMallocBuffer)
            fastFree(pMallocBuffer);
#endif	
		
       
    }

    if(flush) // If flush is true, then this is the last call, so we unilaterally clear our buffer.
        m_numBufferedBytes = 0;

    return resultString;
}


CString TextCodecICU::encode(const UChar* characters, size_t length, UnencodableHandling /*handling*/)
{
    CString result;

    if (length)
    {
        #if defined(_WIN32) || defined(EA_PLATFORM_UNIX)
            const size_t kMaxStackSize = 4096;
        #else
            const size_t kMaxStackSize = 1024;
        #endif
        char* pMallocBuffer = NULL;
        
		size_t destLen = (EA::IO::StrlcpyUTF16ToUTF8(NULL, 0, characters, length))+1; // Call with NULL in order to get the required strlen., //+1 because a null character is appended by the function at the end.
		char* pUsedBuffer   = (destLen < kMaxStackSize) ? (char*)alloca(destLen) : (pMallocBuffer = (char*)fastMalloc(destLen));
		EA::IO::StrlcpyUTF16ToUTF8(pUsedBuffer, destLen, characters, length);

	    result = CString(pUsedBuffer, destLen-1);//subtract the NULL character, string would append it.

        if(pMallocBuffer)
            fastFree(pMallocBuffer);
    }

    return result;
}





} // namespace WebCore
