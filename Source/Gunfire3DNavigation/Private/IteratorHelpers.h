// Copyright Gunfire Games, LLC. All Rights Reserved.

#pragma once

//
// A helper for only returning values in an array that pass a check.
// Todo: Should templatize the condition, right now it expects a function called IsActive.
//
template<typename ValueType>
class FConditionalRangeIterator
{
public:
	FConditionalRangeIterator(ValueType* BeginIn, ValueType* EndIn)
		: Begin(BeginIn), End(EndIn)
	{
	}

	struct FRangedForIterator
	{
		explicit FRangedForIterator(ValueType* BeginIn, ValueType* EndIn)
			: Cur(BeginIn), End(EndIn)
		{
			while (Cur != nullptr && Cur != End && !Cur->IsActive())
			{
				++Cur;
			}
		}

		ValueType& operator*() const
		{
			return *Cur;
		}

		FRangedForIterator& operator++()
		{
			++Cur;

			while (Cur != End && !Cur->IsActive())
			{
				++Cur;
			}

			return *this;
		}

		bool Valid() const
		{
			return Cur != End;
		}

		friend bool operator!=(const FRangedForIterator& A, const FRangedForIterator& B)
		{
			return A.Valid();
		}

	private:
		ValueType* Cur;
		ValueType* End;
	};

	FORCEINLINE FRangedForIterator		begin() { return FRangedForIterator(Begin, End); }
	FORCEINLINE FRangedForIterator		end() { return FRangedForIterator(nullptr, nullptr); }

private:
	ValueType* Begin;
	ValueType* End;
};
