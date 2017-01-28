#pragma once
#include <WiFiClientSecure.h>

class WiFiClientSecureRedirect : public WiFiClientSecure {

  public:
    WiFiClientSecureRedirect();
    ~WiFiClientSecureRedirect();

	// housekeeping, call at least every 100 msec from loop()
	void tick();

	// async connect over HTTPS
	int connect(char const * const host, uint16_t const port);

	// poll to see if connected over HTTPS
	uint8_t connected();

	// request data over HTTPS where host uses redirection
	uint8_t const request(char const * const dstPath,
		                  char const * const dstHost,
		                  uint32_t const timeout_ms,
		                  char const * const dstFingerprint,
		                  char const * const redirFingerprint);

	// poll to see if redirection host responded
	int  response();

    // poll to see if data is available from the redir host
	int available();

    // close the connection
	void stop();

  private:
	char const * dstPath;
	char const * dstHost;
	uint16_t dstPort;
	uint32_t timeout_ms;
	char const * dstFingerprint;
	char const * redirFingerprint;
	char redirHost[30];
	char redirPath[300];
	uint16_t redirPort;
	uint32_t beginWait;

	typedef enum { 
		IDLE = 0, 
		HOST_WAIT4CONNECTION, 
		HOST_CONNECTED, 
		HOST_WAIT4REPLY, 
		REDIR_WAIT4CONNECTION, 
		REDIR_CONNECTED, 
		REDIR_WAIT4REPLY, 
		AVAILABLE, 
		COUNT // terminator
	} state_t;
	uint32_t eventTimeouts[COUNT] = {
		0,     // IDLE (0)
		1000,  // HOST_WAIT4CONNECTION (1)
		0,     // HOST_CONNECTED (2)
		10000, // HOST_WAIT4REPLY (3)
		1000,  // REDIR_WAIT4CONNECTION (4)
		100,   // REDIR_CONNECTED (5)
		10000, // REDIR_WAIT4REPLY (6)
		0,     // AVAILABLE
	};
	state_t state = IDLE;
	uint8_t const sendHostRequest();
	uint8_t const receiveHostReply();
	uint8_t const sendRedirRequest();
	uint8_t const receiveRedirHeader();
};