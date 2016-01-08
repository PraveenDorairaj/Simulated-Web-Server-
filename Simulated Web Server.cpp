#include <iostream>
#include "ece150-proj3-provided.h"

using namespace std;


struct priorityQueue{
	float value;
	priorityQueue *next;
};

int convertTimeSec(timestamp t){
	return t.hh*3600 + t.mm*60 + t.ss;
}

float priority(uri &inputList,uri*input,char*name) {
	while(input){
		if (input->name == name){
			inputList = *input;
		}
		input = input->next;
	}
}

float size(uri &inputList,uri*input,char*name) {
	while(input){
		if (input->name == name){
			inputList = *input;
		}
		input = input->next;
	}
}

timestamp maxClientDelay(uri*uriServer,request*b) {	
	float severTime = 0;
	priorityQueue *list = 0;
	request *listQueue = 0;
	
	do{
		
		while(b){
			
			if (convertTimeSec(b->t) > severTime) break;
			
			request *a = new request;
			a->name = b->name;
			a->t = b->t;
			a->next = listQueue;
			listQueue = a;
			
			b = b->next;
		}
		
		if (!listQueue){
			severTime = convertTimeSec(b->t);
			continue;
		}
		
		request *front = listQueue;
		request *back = listQueue;
		
		request *ordered = listQueue;
		
		uri inputList;
		priority(inputList, uriServer, listQueue->name);
		float maxPriority = inputList.priority;
		request *count = listQueue;
		
		while (count){
			priority(inputList, uriServer, count->name);
			if (inputList.priority > maxPriority){
				maxPriority = inputList.priority;
				ordered = count;
				front = back;
			}
			back = count;
			count = count ->next;
		}
		
		float delayTime = severTime - convertTimeSec(ordered->t);
		priorityQueue *temp = new priorityQueue;
		temp->value = delayTime;
		temp->next = list;
		list = temp;
		
		size(inputList, uriServer, ordered->name);
		severTime += (float)inputList.size * 0.8;
		
		if (ordered == listQueue){
			listQueue = ordered->next;
		}else{
			front->next = ordered->next;
		}
	}while(listQueue||b);
	
	int maxDelay = 0;
	
	while(list) {
		if (list -> value > maxDelay){
			maxDelay = list -> value;
		}
		list = list->next;
	}
	
	timestamp maxDelayTime;
	maxDelayTime.hh = maxDelay/3600;
	maxDelay = maxDelay%3600;
	maxDelayTime.mm = maxDelay/60;
	maxDelayTime.ss = maxDelay%60;
	
	return maxDelayTime;
}

