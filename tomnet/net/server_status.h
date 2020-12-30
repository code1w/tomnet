// Modified from evpp project https://github.com/Qihoo360/evpp/

#ifndef  NET_SERVER_STATUS_H
#define  NET_SERVER_STATUS_H

#include <atomic>
#include <string>

namespace tom {
#ifndef H_CASE_STRING_BIGIN
#define H_CASE_STRING_BIGIN(state) switch(state){
#define H_CASE_STRING(state) case state:return #state;break;
#define H_CASE_STRING_END()  default:return "Unknown";break;}
#endif
	class ServerStatus {
	public:
		enum Status {
			kNull = 0,
			kInitializing = 1,
			kInitialized = 2,
			kStarting = 3,
			kRunning = 4,
			kStopping = 5,
			kStopped = 6,
		};

		enum SubStatus {
			kSubStatusNull = 0,
			kStoppingListener = 1,
			kStoppingThreadPool = 2,
		};

		std::string StatusToString() const {
			H_CASE_STRING_BIGIN(status_.load());
			H_CASE_STRING(kNull);
			H_CASE_STRING(kInitialized);
			H_CASE_STRING(kRunning);
			H_CASE_STRING(kStopping);
			H_CASE_STRING(kStopped);
			H_CASE_STRING_END();
		}

		bool IsRunning() const {
			return status_.load() == kRunning;
		}

		bool IsStopped() const {
			return status_.load() == kStopped;
		}

		bool IsStopping() const {
			return status_.load() == kStopping;
		}

	protected:
		std::atomic<Status> status_ = { kNull };
		std::atomic<SubStatus> substatus_ = { kSubStatusNull };
	};

}
#endif //  TOM_NET_SERVER_STATUS_H