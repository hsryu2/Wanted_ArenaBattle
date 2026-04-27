// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/ABGameSingleton.h"

UABGameSingleton::UABGameSingleton()
{
	// 데이터 테이블 에셋 로드.
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(
		TEXT("")
	);

	// 에셋 로드 성공 시 섲엉.
	if (DataTableRef.Succeeded())
	{
		// UDataTable 변수 저장.
		const UDataTable* DataTable = DataTableRef.Object;
		// 유효성 검사.
		ensureAlways(DataTable->GetRowMap().Num() > 0);
		
		// Value 값만 배열에 저장하기 위한 배열.
		TArray<uint8*> ValueArray;

		// Value 값만 배열에 저장.
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		
		// for-loop 해도 됨.
		// 편한거 사용할 것.
		// 람다보다 루프가 조금 더 빠름 람다는 임시 객체를 만들어서 넘겨주는 작업들이 필요하기 때문.
		// 무시해도 될 정도의 성능차이라서 편한거 사용하는 것.
		//for (uint8* Value : ValueArray)
		//{
		//	CharacterStatTable.Add(
		//		*reinterpret_cast<FABCharacterStat*>(Value)
		//	);
		//}

		// 알고리즘을 활용해 원하는 타입으로 변환.
		Algo::Transform(ValueArray, CharacterStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FABCharacterStat*>(Value);
			}
		);

		// 스텟 테이블의 개수를 최대 레벨로 설정.
		CharacterMaxLevel = CharacterStatTable.Num();

		// 문제 없는지 확인.
		CharacterMaxLevel = CharacterStatTable.Num();
		ensureAlways(CharacterMaxLevel > 0);

	}
}
