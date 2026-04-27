// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/ABGameSingleton.h"

UABGameSingleton::UABGameSingleton()
{
	// 데이터 테이블 에셋 로드.
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(
		TEXT("/Game/ArenaBattle/GameData/ABCharacterStatTable.ABCharacterStatTable")
	);

	// 에셋 로드 성공 시 설정. 
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


UABGameSingleton& UABGameSingleton::Get()
{
	// 프로젝트 세팅에서 싱글톤으로 지정하면,
	// 엔진 실행 시 GameSingleton 변수에 생성해줌.
	// CastChecked -> 형변환 실패 시 오류를 반환해줌.
	UABGameSingleton* Singleton = CastChecked<UABGameSingleton>(GEngine->GameSingleton);

	if (Singleton)
	{
		return *Singleton;

	}

	// 오류 상황.
	// 제대로 된 경우에 아래에 접근하면 안됨.
	UE_LOG(LogTemp, Error, TEXT("Invalid Game Sington"));

	// 함수 구현을 위해 반환이 필요함.
	return *NewObject< UABGameSingleton>();
}