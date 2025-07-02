// Copyright 2017-2021 Brace Yourself Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorStyleSet.h"
#include "SlateFwd.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Input/Reply.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/STableViewBase.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Images/SThrobber.h"
#include "BYGLocalization/Public/BYGLocalization.h"
#include "BYGLocalization/Private/BYGParseFileRunnable.h"

#define LOCTEXT_NAMESPACE "BYGLocalization"

class FBYGLocalizationStatEntry
{
public:
	static TSharedRef<FBYGLocalizationStatEntry> Create()
	{
		return MakeShareable( new FBYGLocalizationStatEntry() );
	}
	FString LocaleCode;
	FText Language;
	FString Path;
	int32 NormalEntries;
	int32 NewEntries;
	int32 ModifiedEntries;
	int32 DeprecatedEntries;
	int32 TotalEntries;
	FText Status;
	bool bIsRefreshing = false;
};

class SBYGLocalizationStatsWindow 
	: public SCompoundWidget
{

public:
	SLATE_BEGIN_ARGS( SBYGLocalizationStatsWindow ){}
	SLATE_END_ARGS()

	virtual ~SBYGLocalizationStatsWindow();

	void Construct( const FArguments& InArgs );

	void OnFileParseComplete( const FString& Path, const BYGLocStats& LocStats );
protected:
	TSharedRef<ITableRow> OnGenerateWidgetForList( TSharedPtr<FBYGLocalizationStatEntry> InItem, const TSharedRef<STableViewBase>& OwnerTable );
	TSharedPtr<SWidget> GetListContextMenu();

	void OpenFolder();
	void OpenFile();
	void RefreshFile();

	void OnDoubleClicked( TSharedPtr<FBYGLocalizationStatEntry> );

	TSharedPtr< SListView< TSharedPtr<FBYGLocalizationStatEntry> > > StatsList;

	TArray< TSharedPtr< FBYGLocalizationStatEntry > > Items;

	TSet< TSharedPtr<FBYGLocalizationStatEntry> > StoredExpandedItems;

	FReply RefreshAll();
	FReply CancelAll();
	void CleanupThreads();
	// Could be TUniquePtr?
	TSharedPtr<FBYGParseFileRunnable> ParseFileRunnable;

	TSharedPtr<SCircularThrobber> StatusThrobber;
};


class SBYGEntryTableRow : public SMultiColumnTableRow< TSharedPtr<FBYGLocalizationStatEntry> >
{
public:

	SLATE_BEGIN_ARGS( SBYGEntryTableRow )
		: _ItemToEdit()
	{
	}
		SLATE_ARGUMENT( TSharedPtr<FBYGLocalizationStatEntry>, ItemToEdit )
	SLATE_END_ARGS()


	virtual TSharedRef<SWidget> GenerateWidgetForColumn( const FName& ColumnName ) override
	{
		FSlateFontInfo ItemEditorFont = FCoreStyle::Get().GetFontStyle( TEXT( "NormalFont" ) );

		// Get # of files in folder
		// Get # of matched files in folder

		#if 0
		if ( ColumnName == TEXT( "Name" ) )
		{
			// The name column is special. In lists it needs to show an expander arrow and be indented
			// in order to give the appearance of a list.

			TSharedRef<SWidget> CellContent =
				SNew( STextBlock )
				.Font( ItemEditorFont )
				.Text( this, &SBYGEntryTableRow::GetLocale );

			if ( OwnerTablePtr.Pin()->GetTableViewMode() == ETableViewMode::Tree )
			{
				// Rows in a tree need to show an SExpanderArrow (it also indents!) to give the appearance of being a tree.
				return
					SNew( SHorizontalBox )
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew( SExpanderArrow, SharedThis( this ) )
					]
				+ SHorizontalBox::Slot()
					.VAlign( VAlign_Center )
					.AutoWidth()
					[
						CellContent
					];
			}
			else
			{
				// Lists do not need an expander arrow
				return CellContent;
			}
		}
		if ( ColumnName == "PrimaryLanguage")
		{
			return
				SNew( SHorizontalBox )
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew( SExpanderArrow, SharedThis( this ) )
				]
			+ SHorizontalBox::Slot()
				.VAlign( VAlign_Center )
				.FillWidth(1.0f)
				[
					SNew(STextBlock)
					.Text( LOCTEXT( "Something", "Something" ) )
				];

		}
		#endif
		if ( ColumnName == TEXT( "PrimaryLanguage" ) )
		{
			return SNew( STextBlock )
			.Font( FAppStyle::Get().GetFontStyle( "FontAwesome.11" ) )
			.TextStyle(FAppStyle::Get(), "ContentBrowser.TopBar.Font")
			.Text( this, &SBYGEntryTableRow::GetPrimaryLanguage );
		}
		else if ( ColumnName == TEXT( "LocaleCode" ) )
		{
			return SNew( STextBlock ).Font( ItemEditorFont ).Text( this, &SBYGEntryTableRow::GetLocaleCode );
		}
		else if ( ColumnName == TEXT( "Language" ) )
		{
			return SNew( STextBlock ).Font( ItemEditorFont ).Text( this, &SBYGEntryTableRow::GetLanguage );
		}
		else if ( ColumnName == TEXT( "Path" ) )
		{
			return SNew( STextBlock ).Font( ItemEditorFont ).Text( this, &SBYGEntryTableRow::GetPath );
		}
		else if ( ColumnName == TEXT( "Normal" ) )
		{
			return SNew( STextBlock ).Font( ItemEditorFont ).Text( this, &SBYGEntryTableRow::GetNormalEntries );
		}
		else if ( ColumnName == TEXT( "New" ) )
		{
			return SNew( STextBlock ).Font( ItemEditorFont ).Text( this, &SBYGEntryTableRow::GetNewEntries );
		}
		else if ( ColumnName == TEXT( "Modified" ) )
		{
			return SNew( STextBlock ).Font( ItemEditorFont ).Text( this, &SBYGEntryTableRow::GetModifiedEntries );
		}
		else if ( ColumnName == TEXT( "Deprecated" ) )
		{
			return SNew( STextBlock ).Font( ItemEditorFont ).Text( this, &SBYGEntryTableRow::GetDeprecatedEntries );
		}
		else if ( ColumnName == TEXT( "Total" ) )
		{
			return SNew( STextBlock ).Font( ItemEditorFont ).Text( this, &SBYGEntryTableRow::GetTotalEntries );
		}
		else if ( ColumnName == TEXT( "Status" ) )
		{
			return SNew( STextBlock ).Font( ItemEditorFont ).Text( this, &SBYGEntryTableRow::GetStatus );
		}
		else
		{
			return
				SNew( STextBlock )
				.Text( FText::Format( LOCTEXT( "UnsupprtedColumnText", "Unsupported Column: {0}" ), FText::FromName( ColumnName ) ) );
		}
	}

	/**
	 * Construct the widget
	 *
	 * @param InArgs   A declaration from which to construct the widget
	 */
	void Construct( const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView )
	{
		ItemToEdit = InArgs._ItemToEdit;

		FSuperRowType::Construct( FSuperRowType::FArguments()
			.Padding( 2 )
			, InOwnerTableView );
	}

	virtual int32 DoesItemHaveChildren() const override { return 1; }
	virtual bool IsItemExpanded() const override { return bIsExpanded; }
	virtual void ToggleExpansion() override;

	const FSlateBrush* GetBackgroundImage() const;
private:
	bool bIsExpanded = true;
	TSharedPtr< SWidget > CategoryContent;

private:

	FText GetPrimaryLanguage() const
	{
		if ( ItemToEdit->LocaleCode == "en" )
			return FText::FromString( FString( TEXT( "\xf005" ) ) ); /*fa-star*/
		return FText::GetEmpty();
	}
	FText GetLocaleCode() const
	{
		return FText::FromString( ItemToEdit->LocaleCode );
	}
	FText GetLanguage() const
	{
		return ItemToEdit->Language;
	}
	FText GetPath() const
	{
		return FText::FromString( ItemToEdit->Path );
	}
	FText GetNormalEntries() const
	{
		return FText::AsNumber( ItemToEdit->NormalEntries );
	}
	FText GetNewEntries() const
	{
		return FText::AsNumber( ItemToEdit->NewEntries );
	}
	FText GetModifiedEntries() const
	{
		return FText::AsNumber( ItemToEdit->ModifiedEntries );
	}
	FText GetDeprecatedEntries() const
	{
		return FText::AsNumber( ItemToEdit->DeprecatedEntries );
	}
	FText GetTotalEntries() const
	{
		return FText::AsNumber( ItemToEdit->TotalEntries );
	}
	FText GetStatus() const
	{
		if ( ItemToEdit->bIsRefreshing )
		{
			return LOCTEXT( "Reloading", "Reloading..." );
		}
		return ItemToEdit->Status;
	}

	/** A pointer to the data item that we visualize/edit */
	TSharedPtr<FBYGLocalizationStatEntry> ItemToEdit;
};

#undef LOCTEXT_NAMESPACE
