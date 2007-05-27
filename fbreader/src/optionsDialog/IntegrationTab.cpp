/*
 * FBReader -- electronic book reader
 * Copyright (C) 2004-2007 Nikolay Pultsin <geometer@mawhrin.net>
 * Copyright (C) 2005 Mikhail Sobolev <mss@mawhrin.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include <ZLOptionsDialog.h>
#include <ZLOptionEntry.h>
#include <optionEntries/ZLToggleBooleanOptionEntry.h>
#include <optionEntries/ZLSimpleOptionEntry.h>
#include <ZLStringUtil.h>

#include "OptionsDialog.h"
#include "../FBOptions.h"

#include "../external/ProgramCollection.h"

class ProgramChoiceEntry : public ZLComboOptionEntry {

public:
	ProgramChoiceEntry(const ProgramCollection &collection);

private:
	const std::vector<std::string> &values() const;
	void onAccept(const std::string &value);
	void onValueSelected(int index);

public:
	const std::string &initialValue() const;
	void addDependentEntry(const std::string &name, ZLOptionEntry *dependentEntry);
	void updateDependentEntries(bool visible);

private:
	const ProgramCollection &myCollection;
	std::string myValue;
	std::map<ZLOptionEntry*,std::string> myDependentEntries;
};

class EnableIntegrationEntry : public ZLToggleBooleanOptionEntry {

public:
	EnableIntegrationEntry(ZLBooleanOption &option);
	void setProgramChoiceEntry(ProgramChoiceEntry *programChoiceEntry);
	void onStateChanged(bool state);

private:
	ProgramChoiceEntry *myProgramChoiceEntry;
};

ProgramChoiceEntry::ProgramChoiceEntry(const ProgramCollection &collection) : myCollection(collection) {
	myValue = initialValue();
}

const std::string &ProgramChoiceEntry::initialValue() const {
	return myCollection.CurrentNameOption.value();
}

const std::vector<std::string> &ProgramChoiceEntry::values() const {
	return myCollection.names();
}

void ProgramChoiceEntry::onAccept(const std::string &value) {
	myCollection.CurrentNameOption.setValue(value);
}

void ProgramChoiceEntry::addDependentEntry(const std::string &name, ZLOptionEntry *dependentEntry) {
	myDependentEntries[dependentEntry] = name;
}

void ProgramChoiceEntry::onValueSelected(int index) {
	myValue = values()[index];
	updateDependentEntries(true);
}

void ProgramChoiceEntry::updateDependentEntries(bool visible) {
	for (std::map<ZLOptionEntry*,std::string>::const_iterator it = myDependentEntries.begin(); it != myDependentEntries.end(); ++it) {
		it->first->setVisible(visible && (it->second == myValue));
	}
}

EnableIntegrationEntry::EnableIntegrationEntry(ZLBooleanOption &option) : ZLToggleBooleanOptionEntry(option), myProgramChoiceEntry(0) {
}

void EnableIntegrationEntry::setProgramChoiceEntry(ProgramChoiceEntry *programChoiceEntry) {
	addDependentEntry(programChoiceEntry);
	myProgramChoiceEntry = programChoiceEntry;
}

void EnableIntegrationEntry::onStateChanged(bool state) {
	ZLToggleBooleanOptionEntry::onStateChanged(state);
	if (myProgramChoiceEntry != 0) {
		myProgramChoiceEntry->updateDependentEntries(state);
	}
}

void OptionsDialog::createIntegrationTab(shared_ptr<ProgramCollection> collection, const ZLResourceKey &key, std::vector<ZLOptionEntry*> &additionalOptions) {
	if (!collection.isNull()) {
		const std::vector<std::string> &programNames = collection->names();
		if (!programNames.empty()) {
			ZLDialogContent &integrationTab = myDialog->createTab(key);
			const std::string name =
				(programNames.size() == 1) ?
					programNames[0] :
					integrationTab.getValue(ZLResourceKey("defaultName"));
			const std::string optionName =
				ZLStringUtil::printf(
					integrationTab.getValue(ZLResourceKey("enableIntegration")), name
				);
			EnableIntegrationEntry *enableIntegrationEntry =
				new EnableIntegrationEntry(collection->EnableCollectionOption);
			integrationTab.addOption(optionName, "", enableIntegrationEntry);

			ProgramChoiceEntry *programChoiceEntry = 0;
			if (programNames.size() > 1) {
				programChoiceEntry = new ProgramChoiceEntry(*collection);
				integrationTab.addOption(ZLResourceKey("choice"), programChoiceEntry);
				enableIntegrationEntry->setProgramChoiceEntry(programChoiceEntry);
			}

			for (std::vector<std::string>::const_iterator it = programNames.begin(); it != programNames.end(); ++it) {
				const std::vector<Program::OptionDescription> &options = collection->program(*it)->options();
				for (std::vector<Program::OptionDescription>::const_iterator jt = options.begin(); jt != options.end(); ++jt) {
					ZLStringOption *parameterOption = new ZLStringOption(FBOptions::EXTERNAL_CATEGORY, *it, jt->OptionName, jt->DefaultValue);
					storeTemporaryOption(parameterOption);
					ZLOptionEntry *parameterEntry = new ZLSimpleStringOptionEntry(*parameterOption);
					if (programChoiceEntry != 0) {
						programChoiceEntry->addDependentEntry(*it, parameterEntry);
					} else {
						enableIntegrationEntry->addDependentEntry(parameterEntry);
					}
					integrationTab.addOption(jt->DisplayName, "", parameterEntry);
				}
			}
			for (std::vector<ZLOptionEntry*>::const_iterator it = additionalOptions.begin(); it != additionalOptions.end(); ++it) {
				enableIntegrationEntry->addDependentEntry(*it);
				integrationTab.addOption("", "", *it);
			}
			enableIntegrationEntry->onStateChanged(enableIntegrationEntry->initialState());
			return;
		}
	}

	for (std::vector<ZLOptionEntry*>::const_iterator it = additionalOptions.begin(); it != additionalOptions.end(); ++it) {
		delete *it;
	}
}
