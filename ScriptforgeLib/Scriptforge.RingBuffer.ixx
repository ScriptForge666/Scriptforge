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
import Scriptforge.Local;
import std;

namespace Scriptforge {
	inline namespace RingBuffer {
		export
			template <typename T = std::string, typename Alloc = std::allocator<T>>
		class RingBuffer {
		public:
			using value_type = T;
			using reference = value_type&;
			using const_reference = const value_type&;
			using size_type = std::size_t;
			using iterator = int;
			using const_iterator = int;
			using difference_type = std::ptrdiff_t;
			using allocator_type = Alloc;

			RingBuffer(const size_type capacity = 50, const Scriptforge::Lang lang = Scriptforge::Lang{});
		private:
			Scriptforge::Lang m_lang;
			size_type m_capacity;
			std::vector<value_type, Alloc> m_buffer;
		};
	}
}

namespace Scriptforge {
	inline namespace RingBuffer {
		template <typename T, typename Alloc>
		RingBuffer<T, Alloc>::RingBuffer(const size_type capacity, const Scriptforge::Lang lang)
			:m_lang(lang) {
			if (capacity == 0) {
				throw Scriptforge::Error{ Scriptforge::ErrCode::toString(Scriptforge::ErrCode::ErrCode::RingBufferCapacityBeZero), "Ring buffer capacity must be greater than 0" };
			}

		}
	}
}