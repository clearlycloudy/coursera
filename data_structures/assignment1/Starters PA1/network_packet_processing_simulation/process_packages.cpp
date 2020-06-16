#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

struct Request {
    Request(int arrival_time, int process_time):
        arrival_time(arrival_time),
        process_time(process_time)
    {}

    int arrival_time;
    int process_time;
};

struct Response {
    Response(bool dropped, int start_time):
        dropped(dropped),
        start_time(start_time)
    {}

    bool dropped;
    int start_time;
};

class Buffer {
public:
    Buffer(int size):
        size_(size),
        finish_time_(),
	time_(0)
    {}

    Response Process(const Request &request) {
	if( finish_time_.size() == size_ ){
	    //queue might be full
	    int popped = 0;
	    int last_done = finish_time_.front();
	    //flush all items with smaller time stamps than request
	    while( !finish_time_.empty() && (finish_time_.front() <= request.arrival_time)){
		last_done = finish_time_.front();
		finish_time_.pop();
		++popped;
	    }
	    if( popped == 0 ){
		return Response( true, request.arrival_time );
	    }else{
		time_ = max( finish_time_.back(), request.arrival_time );
	        int begin_process = time_;
		time_ = time_ + request.process_time;
		finish_time_.push(time_);
		return { false, begin_process };
	    }
	}else{
	    //queue is not full, so add to queue
	    time_ = max( time_, request.arrival_time );
	    int begin_process = time_;
	    time_ = time_ + request.process_time;
	    finish_time_.push(time_);
	    return { false, begin_process };
	}
    }
private:
    int size_;
    std::queue <int> finish_time_;
    int time_;
};

std::vector <Request> ReadRequests() {
    std::vector <Request> requests;
    int count;
    std::cin >> count;
    for (int i = 0; i < count; ++i) {
        int arrival_time, process_time;
        std::cin >> arrival_time >> process_time;
        requests.push_back(Request(arrival_time, process_time));
    }
    return requests;
}

std::vector <Response> ProcessRequests(const std::vector <Request> &requests, Buffer *buffer) {
    std::vector <Response> responses;
    int time = 0;
    for (int i = 0; i < requests.size(); ++i){
        responses.push_back(buffer->Process(requests[i]));
    }
    return responses;
}

void PrintResponses(const std::vector <Response> &responses) {
    for (int i = 0; i < responses.size(); ++i)
        std::cout << (responses[i].dropped ? -1 : responses[i].start_time) << std::endl;
}

int main() {
    int size;
    std::cin >> size;
    std::vector <Request> requests = ReadRequests();

    Buffer buffer(size);
    std::vector <Response> responses = ProcessRequests(requests, &buffer);

    PrintResponses(responses);
    return 0;
}
