/*
* DASHStream.h
*****************************************************************************
* Copyright (C) 2015, liberty_developer
*
* Email: liberty.developer@xmail.net
*
* This source code and its use and distribution, is subject to the terms
* and conditions of the applicable license agreement.
*****************************************************************************/

#pragma once

#include "DASHTree.h"
#include <string>

namespace dash
{
	class DASHStream;

	class DASHStreamObserver
	{
	public:
		virtual void OnStreamChange(DASHStream *stream, uint32_t segment)=0;
	};

	class DASHStream
	{
	public:
		DASHStream(DASHTree &tree,DASHTree::StreamType type);
		~DASHStream();
		void set_observer(DASHStreamObserver *observer){ observer_ = observer; };
		bool prepare_stream(const uint32_t width, const uint32_t height, const char *lang, uint32_t fixed_bandwidth);
		bool start_stream(const uint32_t seg_offset);
		bool select_stream(bool force = false);
		void stop(){ stopped_ = true; };
		void clear();
		void info(std::ostream &s);

		void set_bandwidth(uint32_t fixed_bandwidth){ fixed_bandwidth_ = fixed_bandwidth; };
		
		unsigned int get_type()const{ return type_; };

		uint32_t read(void* buffer,
			uint32_t  bytesToRead);
		uint64_t tell(){ return absolute_position_;};
		bool seek(uint64_t const pos);
		DASHTree::Representation const *getRepresentation(){ return current_rep_; };
	private:
		bool download_segment();

		DASHTree &tree_;
		DASHTree::StreamType type_;
		DASHStreamObserver *observer_;
		// Active configuration
		const DASHTree::Period *current_period_;
		const DASHTree::AdaptationSet *current_adp_;
		const DASHTree::Representation *current_rep_;
		const DASHTree::Segment *current_seg_;
		//We assume that a single segment can build complete frames
		std::string segment_buffer_;
		std::size_t segment_read_pos_;
		uint64_t absolute_position_;
		
		void *curl_handle_;

		uint16_t width_, height_;
		std::string language_;
		uint32_t fixed_bandwidth_;
		double download_speed_;

		bool stopped_;
	};
};
