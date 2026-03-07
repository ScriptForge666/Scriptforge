// Copyright 2026 Scriptforge
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//     http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
export module Scriptforge.RingBuffer;

import Scriptforge.Err;
import Scriptforge.ErrCode;
import Scriptforge.Msg;
import Scriptforge.Local;
import std;

namespace Scriptforge {
	inline namespace RingBuffer {
		export 
			template<typename RingBufferType>
		class ConstRingBufferIterator {
		public:
			using value_type = typename RingBufferType::value_type;
			using difference_type = typename RingBufferType::difference_type;
			using iterator_category = std::random_access_iterator_tag;
			using reference = const value_type&;
			using pointer = const value_type*;
			using node_container_iterator = typename RingBufferType::node_container_iterator::const_iterator;
			using size_type = typename RingBufferType::size_type;
			
		};
		export
			template <typename T = Scriptforge::Message<>, typename Container = std::vector<T, std::allocator<T>>>
		class RingBuffer {
		public:
			using value_type = T;
			using reference = value_type&;
			using const_reference = const value_type&;
			using size_type = std::size_t;
			using iterator = int;
			using const_iterator = ConstRingBufferIterator<RingBuffer<value_type, Container>>;
			using difference_type = std::ptrdiff_t;

			RingBuffer(const size_type capacity = 50, const Scriptforge::Lang lang = Scriptforge::Lang{});
			value_type size() const noexcept;
			value_type max_size() const noexcept;
			bool empty() const noexcept;
			bool operator==(const RingBuffer& other) const = default;
			bool operator!=(const RingBuffer& other) const = default;

		private:
			friend class ConstRingBufferIterator<RingBuffer<value_type, Container>>;
			Scriptforge::Lang m_lang;
			size_type m_capacity;
			Container m_buffer;
		};
	}
}

namespace Scriptforge {
	inline namespace RingBuffer {
		template <typename T, typename Container>
		RingBuffer<T, Container>::RingBuffer(const size_type capacity, const Scriptforge::Lang lang)
			:m_lang(lang) {
			if (capacity == 0) {
				Scriptforge::ErrCode::throwError(Scriptforge::ErrCode::ErrCode::RingBufferCapacityBeZero, __func__, m_lang);
			}
		}

		template <typename T, typename Container>
		RingBuffer<T, Container>::value_type RingBuffer<T, Container>::size() const noexcept {
			return m_buffer.size();
		}

		template <typename T, typename Container>
		RingBuffer<T, Container>::value_type RingBuffer<T, Container>::max_size() const noexcept {
			return m_capacity;
		}

		template <typename T, typename Container>
		bool RingBuffer<T, Container>::empty() const noexcept {
			return m_buffer.empty();
		}


	}
}