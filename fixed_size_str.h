#pragma once

#include <string_view>
#include <algorithm>

namespace fls
{
	template <class CharT
		, std::size_t max_length
		, class Traits = std::char_traits<CharT>>
		class basic_str
	{
	public:
		constexpr basic_str() noexcept {}

		constexpr basic_str(const CharT* str) noexcept
			:active_length_(max_length)
		{
			active_length_ = std::min(Traits::length(str), max_length);
			std::copy(str, str + active_length_, buffer_);
		}

		constexpr basic_str(const CharT* str, std::size_t length) noexcept
		{
			active_length_ = std::min(length, max_length);
			std::copy(str, str + active_length_, buffer_);
		}

		constexpr const CharT* c_str() const noexcept { return buffer_; }

		constexpr std::string_view str() const noexcept
		{
			return std::string_view(buffer_, active_length_ + 1);
		}

		constexpr auto length() const { return active_length_; }

		constexpr auto max_size() const { return max_length; }

		constexpr void reset() noexcept
		{
			active_length_ = 0;
			buffer_[0] = '\0';
		}

		void append(const CharT* str) noexcept
		{
			auto to_copy = std::min(Traits::length(str), (max_length - active_length_));
			std::memcpy(buffer_ + active_length_, str, to_copy);
			active_length_ += to_copy;
		}

		void append(const CharT* str, std::size_t length) noexcept
		{
			auto to_copy = std::min(length, (max_length - active_length_));
			std::memcpy(buffer_ + active_length_, str, to_copy);
			active_length_ += to_copy;
		}

		constexpr bool operator==(const basic_str& rhs) const
		{
			return (max_size() == rhs.max_size())
				&& (length() == rhs.length())
				&& std::equal(buffer_, buffer_ + length(), rhs.buffer_);
		}

		constexpr bool operator!=(const basic_str& rhs) const
		{
			return !(*this == rhs);
		}

	private:
		std::size_t active_length_{ 0 };
		CharT buffer_[max_length + 1]{};
	};

	template <std::size_t max_length>
	using fixed_size_str = fls::basic_str<char, max_length>;

	template <std::size_t max_length>
	using fixed_size_wstr = fls::basic_str<wchar_t, max_length>;

	template <std::size_t max_length>
	using fixed_size_u8str = fls::basic_str<char8_t, max_length>;

	template <std::size_t max_length>
	using fixed_size_u16str = fls::basic_str<char16_t, max_length>;

	template <std::size_t max_length>
	using fixed_size_u32str = fls::basic_str<char32_t, max_length>;
}
