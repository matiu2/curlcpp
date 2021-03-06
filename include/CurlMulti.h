/* 
 * File:   CurlMulti.h
 * Author: Giuseppe
 *
 * Created on March 25, 2014, 11:02 PM
 */

#ifndef CURLMULTI_H
#define	CURLMULTI_H

#include "CurlEasy.h"

using curl::CurlEasy;

namespace curl {
    class CurlMulti : public CurlInterface<CURLM,CURLMcode> {
    public:
        class CurlMessage {
        public:
            CurlMessage(CURLMSG message, CurlEasy easy, void *whatever, CURLcode result) : message(message), easy(easy), whatever(whatever), result(result) {};
            // Returns the message
            inline const CURLMSG getMessage() const noexcept {
                return this->message;
            }
            // Returns the exit code
            inline const CURLcode getResult() const noexcept {
                return this->result;
            }
            // Returns the curl easy handler
            inline const CurlEasy getCurl() const noexcept {
                return this->easy;
            }
            // Returns other stuff.
            inline const void *getWhatever() const noexcept {
                return this->whatever;
            }
        private:
            const CURLMSG message;
            const CurlEasy easy;
            const void *whatever;
            const CURLcode result;
        };
        CurlMulti();
        CurlMulti(const long);
        ~CurlMulti();
        template<typename T> void add(const CurlPair<CURLMoption,T> &);
        template<typename T> void add(const vector<CurlPair<CURLMoption,T>> &);
        template<typename T> void add(const list<CurlPair<CURLMoption,T>> &);
        CurlMulti &addHandle(const CurlEasy &) noexcept;
        CurlMulti &addHandle(const vector<CurlEasy> &) noexcept;
        CurlMulti &removeHandle(const CurlEasy &) noexcept;
        bool perform();
        bool socketAction(curl_socket_t, int, int *);
        const vector<CurlMessage> infoRead();
        void fdSet(fd_set *, fd_set *, fd_set *, int *);
        void timeout(long *);
        void assign(curl_socket_t, void *);
        void wait(curl_waitfd [], unsigned int, int, int *);
        const int getActiveTransfers() const noexcept;
        const int getMessageQueued() const noexcept;
    protected:
        const string toString(const CURLMcode) const noexcept;
    private:
        int message_queued;
        int active_transfers;
        vector<CurlEasy> handlers;
    };
    
    // Implementation of add method
    template<typename T> void CurlMulti::add(const CurlPair<CURLMoption,T> &pair) {
        CURLMcode code = curl_multi_setopt(this->getCurl(),pair.first(),pair.second());
        if (code != CURLM_OK) {
            throw CurlError(this->toString(code));
        }
    }
    // Implementation of add overloaded method
    template<typename T> void CurlMulti::add(const vector<CurlPair<CURLMoption,T>> &pairs) {
        for_each(pairs.begin(),pairs.end(),[this](CurlPair<CURLMoption,T> option) { this->add(option); } );
    }
    // Implementation of add overloaded method
    template<typename T> void CurlMulti::add(const list<CurlPair<CURLMoption,T>> &pairs) {
        for_each(pairs.begin(),pairs.end(),[this](CurlPair<CURLMoption,T> option) { this->add(option); } );
    }
}





#endif	/* CURLMULTI_H */