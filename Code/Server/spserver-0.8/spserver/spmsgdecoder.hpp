/*
 * Copyright 2007 Stephen Liu
 * For license terms, see the file COPYING along with this library.
 */

#ifndef __spmsgdecoder_hpp__
#define __spmsgdecoder_hpp__

class SP_Buffer;

class SP_MsgDecoder {
public:
	virtual ~SP_MsgDecoder();

	enum { eOK, eMoreData };

	virtual int decode( SP_Buffer * inBuffer ) = 0;
};

class SP_DefaultMsgDecoder : public SP_MsgDecoder {
public:
	SP_DefaultMsgDecoder();
	virtual ~SP_DefaultMsgDecoder();

	// always return SP_MsgDecoder::eOK, move buffer from inBuffer to mBuffer
	virtual int decode( SP_Buffer * inBuffer );

	SP_Buffer * getMsg();

private:
	SP_Buffer * mBuffer;
};

class SP_LineMsgDecoder : public SP_MsgDecoder {
public:
	SP_LineMsgDecoder();
	virtual ~SP_LineMsgDecoder();

	// return SP_MsgDecoder::eMoreData until meet <CRLF>
	virtual int decode( SP_Buffer * inBuffer );

	const char * getMsg();

private:
	char * mLine;
};

class SP_DotTermMsgDecoder : public SP_MsgDecoder {
public:
	SP_DotTermMsgDecoder();
	virtual ~SP_DotTermMsgDecoder();

	// return SP_MsgDecoder::eMoreData until meet <CRLF>.<CRLF>
	virtual int decode( SP_Buffer * inBuffer );

	const char * getMsg();

private:
	char * mBuffer;
};

#endif

