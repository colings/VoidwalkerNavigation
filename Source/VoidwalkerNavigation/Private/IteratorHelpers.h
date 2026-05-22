// Copyright Gunfire Games, LLC. All Rights Reserved.
// Copyright Colin Bonstead. All Rights Reserved.

#pragma once

//
// A helper for only returning values in a range that pass a check.
//
template<typename ValueType, typename PredicateType>
class FConditionalRangeIterator
{
public:
	FConditionalRangeIterator(ValueType* BeginIn, ValueType* EndIn, PredicateType InPredicate = PredicateType())
	   : Begin(BeginIn), End(EndIn), Predicate(InPredicate)
	{
	}

	struct FRangedForIterator
	{
		explicit FRangedForIterator(ValueType* BeginIn, ValueType* EndIn, PredicateType InPredicate)
		   : Cur(BeginIn), End(EndIn), Predicate(InPredicate)
		{
			while (Cur != nullptr && Cur != End && !Predicate(*Cur))
			{
				++Cur;
			}
		}

		ValueType& operator*() const { return *Cur; }

		FRangedForIterator& operator++()
		{
			++Cur;
			while (Cur != End && !Predicate(*Cur))
			{
				++Cur;
			}
			return *this;
		}

		bool Valid() const { return Cur != End; }

		friend bool operator!=(const FRangedForIterator& A, const FRangedForIterator& B)
		{
			return A.Valid();
		}

	private:
		ValueType* Cur;
		ValueType* End;
		PredicateType Predicate;
	};

	FORCEINLINE FRangedForIterator begin() { return FRangedForIterator(Begin, End, Predicate); }
	FORCEINLINE FRangedForIterator end() { return FRangedForIterator(nullptr, nullptr, Predicate); }

private:
	ValueType* Begin;
	ValueType* End;
	PredicateType Predicate;
};
