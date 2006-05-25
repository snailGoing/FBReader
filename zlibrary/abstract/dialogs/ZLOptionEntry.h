/*
 * Copyright (C) 2004-2006 Nikolay Pultsin <geometer@mawhrin.net>
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

#ifndef __ZLOPTIONENTRY_H__
#define __ZLOPTIONENTRY_H__

#include <vector>
#include <string>

#include <abstract/ZLOptions.h>

enum ZLOptionKind {
	CHOICE,
	BOOLEAN,
	STRING,
	SPIN,
	COMBO,
	COLOR,
	KEY,
	UNKNOWN,
};

class ZLOptionEntry;

class OptionView {

public:
	OptionView(ZLOptionEntry *option) DIALOG_SECTION;
	virtual ~OptionView() DIALOG_SECTION;
	virtual void setVisible(bool visible) DIALOG_SECTION = 0;
	virtual void setActive(bool active) DIALOG_SECTION = 0;

protected:
	ZLOptionEntry *myOption;
};

class ZLOptionEntry {

public:
	ZLOptionEntry() DIALOG_SECTION;
	virtual ~ZLOptionEntry() DIALOG_SECTION;
	virtual ZLOptionKind kind() const DIALOG_SECTION = 0;
	virtual const std::string &name() const DIALOG_SECTION = 0;

	void setView(OptionView *view) DIALOG_SECTION;

	void setVisible(bool visible) DIALOG_SECTION;
	bool isVisible() const DIALOG_SECTION;

	void setActive(bool active) DIALOG_SECTION;
	bool isActive() const DIALOG_SECTION;

private:
	OptionView *myView;
	bool myIsVisible;
	bool myIsActive;
};

class ZLUserDefinedOptionEntry : public ZLOptionEntry {

protected:
	ZLUserDefinedOptionEntry() DIALOG_SECTION;

public:
	virtual ~ZLUserDefinedOptionEntry() DIALOG_SECTION;
	ZLOptionKind kind() const DIALOG_SECTION;
	virtual OptionView *createView() DIALOG_SECTION = 0;
};

class ZLChoiceOptionEntry : public ZLOptionEntry {

protected:
	ZLChoiceOptionEntry() DIALOG_SECTION;

public:
	virtual ~ZLChoiceOptionEntry() DIALOG_SECTION;
	ZLOptionKind kind() const DIALOG_SECTION;

	virtual const std::string &text(int index) const DIALOG_SECTION = 0;
	virtual int choiceNumber() const DIALOG_SECTION = 0;
	virtual int initialCheckedIndex() const DIALOG_SECTION = 0;
	virtual void onAccept(int index) DIALOG_SECTION = 0;
};

class ZLStringOptionEntry : public ZLOptionEntry {

protected:
	ZLStringOptionEntry() DIALOG_SECTION;

public:
	virtual ~ZLStringOptionEntry() DIALOG_SECTION;
	ZLOptionKind kind() const DIALOG_SECTION;

	virtual const std::string &initialValue() const DIALOG_SECTION = 0;
	virtual void onAccept(const std::string &value) DIALOG_SECTION = 0;
};

class ZLSimpleStringOptionEntry : public ZLStringOptionEntry {

public:
	ZLSimpleStringOptionEntry(const std::string &name, ZLStringOption &option) DIALOG_SECTION;
	virtual ~ZLSimpleStringOptionEntry() DIALOG_SECTION;
	const std::string &name() const DIALOG_SECTION;
	const std::string &initialValue() const DIALOG_SECTION;
	void onAccept(const std::string &value) DIALOG_SECTION;

private:
	std::string myName;
	ZLStringOption &myOption;
};

class ZLBooleanOptionEntry : public ZLOptionEntry {

protected:
	ZLBooleanOptionEntry() DIALOG_SECTION;

public:
	virtual ~ZLBooleanOptionEntry() DIALOG_SECTION;
	ZLOptionKind kind() const DIALOG_SECTION;

	virtual bool initialState() const DIALOG_SECTION = 0;
	virtual void onValueChange(bool state) DIALOG_SECTION;
	virtual void onAccept(bool state) DIALOG_SECTION = 0;
};

class ZLSimpleBooleanOptionEntry : public ZLBooleanOptionEntry {

public:
	ZLSimpleBooleanOptionEntry(const std::string &name, ZLBooleanOption &option) DIALOG_SECTION;
	virtual ~ZLSimpleBooleanOptionEntry() DIALOG_SECTION;
	const std::string &name() const DIALOG_SECTION;
	bool initialState() const DIALOG_SECTION;
	void onAccept(bool state) DIALOG_SECTION;

private:
	std::string myName;
	ZLBooleanOption &myOption;
};

class ZLSpinOptionEntry : public ZLOptionEntry {

protected:
	ZLSpinOptionEntry() DIALOG_SECTION;

public:
	virtual ~ZLSpinOptionEntry() DIALOG_SECTION;
	ZLOptionKind kind() const DIALOG_SECTION;

	virtual int initialValue() const DIALOG_SECTION = 0;
	virtual int minValue() const DIALOG_SECTION = 0;
	virtual int maxValue() const DIALOG_SECTION = 0;
	virtual int step() const DIALOG_SECTION = 0;
	virtual void onAccept(int value) DIALOG_SECTION = 0;
};

class ZLSimpleSpinOptionEntry : public ZLSpinOptionEntry {

public:
	ZLSimpleSpinOptionEntry(const std::string &name, ZLIntegerRangeOption &option, int step) DIALOG_SECTION;
	virtual ~ZLSimpleSpinOptionEntry() DIALOG_SECTION;
	const std::string &name() const DIALOG_SECTION;
	int initialValue() const DIALOG_SECTION;
	int minValue() const DIALOG_SECTION;
	int maxValue() const DIALOG_SECTION;
	int step() const DIALOG_SECTION;
	void onAccept(int value) DIALOG_SECTION;

private:
	std::string myName;
	ZLIntegerRangeOption &myOption;
	int myStep;
};

class ZLComboOptionEntry : public ZLOptionEntry {

protected:
	ZLComboOptionEntry() DIALOG_SECTION;

public:
	virtual ~ZLComboOptionEntry() DIALOG_SECTION;
	ZLOptionKind kind() const DIALOG_SECTION;

	virtual const std::string &initialValue() const DIALOG_SECTION = 0;
	virtual const std::vector<std::string> &values() const DIALOG_SECTION = 0;
	virtual void onValueChange(const std::string&) DIALOG_SECTION;
	virtual void onAccept(const std::string &value) DIALOG_SECTION = 0;
};

class ZLSimpleBoolean3OptionEntry : public ZLComboOptionEntry {

public:
	ZLSimpleBoolean3OptionEntry(const std::string &name, ZLBoolean3Option &option) DIALOG_SECTION;
	virtual ~ZLSimpleBoolean3OptionEntry() DIALOG_SECTION;
	const std::string &name() const DIALOG_SECTION;
	const std::string &initialValue() const DIALOG_SECTION;
	const std::vector<std::string> &values() const DIALOG_SECTION;
	void onAccept(const std::string &value) DIALOG_SECTION;

private:
	static std::vector<std::string> ourValues;
	std::string myName;
	ZLBoolean3Option &myOption;
};

class ZLColorOptionEntry : public ZLOptionEntry {

protected:
	ZLColorOptionEntry() DIALOG_SECTION;

public:
	virtual ~ZLColorOptionEntry() DIALOG_SECTION;
	ZLOptionKind kind() const DIALOG_SECTION;

	virtual const ZLColor color() const DIALOG_SECTION = 0;
	virtual void onAccept(ZLColor color) DIALOG_SECTION = 0;
};

class ZLKeyOptionEntry : public ZLOptionEntry {

public:
	ZLKeyOptionEntry(const std::string &name) DIALOG_SECTION;
	~ZLKeyOptionEntry() DIALOG_SECTION;
	void addActionName(const std::string &actionName) DIALOG_SECTION;
	const std::vector<std::string> &actionNames() const DIALOG_SECTION;
	virtual void onAccept() DIALOG_SECTION = 0;
	virtual int actionIndex(const std::string &key) DIALOG_SECTION = 0;
	virtual void onValueChange(const std::string &key, int index) DIALOG_SECTION = 0;
	//virtual void reset() = 0;

protected:
	ZLOptionKind kind() const DIALOG_SECTION;
	const std::string &name() const DIALOG_SECTION;

private:
	std::string myName;
	std::vector<std::string> myActionNames;
};

class ZLSimpleColorOptionEntry : public ZLColorOptionEntry {

public:
	ZLSimpleColorOptionEntry(ZLColorOption &option) DIALOG_SECTION;
	virtual ~ZLSimpleColorOptionEntry() DIALOG_SECTION;
	const std::string &name() const DIALOG_SECTION;
	const ZLColor color() const DIALOG_SECTION;
	void onAccept(ZLColor color) DIALOG_SECTION;

private:
	ZLColorOption &myOption; 
};

inline ZLOptionEntry::ZLOptionEntry() : myView(0), myIsVisible(true), myIsActive(true) {}
inline ZLOptionEntry::~ZLOptionEntry() {}
inline void ZLOptionEntry::setView(OptionView *view) { myView = view; }
inline bool ZLOptionEntry::isVisible() const { return myIsVisible; }
inline bool ZLOptionEntry::isActive() const { return myIsActive; }

inline ZLUserDefinedOptionEntry::ZLUserDefinedOptionEntry() {}
inline ZLUserDefinedOptionEntry::~ZLUserDefinedOptionEntry() {}
inline ZLOptionKind ZLUserDefinedOptionEntry::kind() const { return UNKNOWN; }

inline ZLChoiceOptionEntry::ZLChoiceOptionEntry() {}
inline ZLChoiceOptionEntry::~ZLChoiceOptionEntry() {}
inline ZLOptionKind ZLChoiceOptionEntry::kind() const { return CHOICE; }

inline ZLStringOptionEntry::ZLStringOptionEntry() {}
inline ZLStringOptionEntry::~ZLStringOptionEntry() {}
inline ZLOptionKind ZLStringOptionEntry::kind() const { return STRING; }

inline ZLSimpleStringOptionEntry::ZLSimpleStringOptionEntry(const std::string &name, ZLStringOption &option) : myName(name), myOption(option) {}
inline ZLSimpleStringOptionEntry::~ZLSimpleStringOptionEntry() {}
inline const std::string &ZLSimpleStringOptionEntry::name() const { return myName; }
inline const std::string &ZLSimpleStringOptionEntry::initialValue() const { return myOption.value(); }
inline void ZLSimpleStringOptionEntry::onAccept(const std::string &value) { myOption.setValue(value); }

inline ZLBooleanOptionEntry::ZLBooleanOptionEntry() {}
inline ZLBooleanOptionEntry::~ZLBooleanOptionEntry() {}
inline ZLOptionKind ZLBooleanOptionEntry::kind() const { return BOOLEAN; }
inline void ZLBooleanOptionEntry::onValueChange(bool) {}

inline ZLSimpleBooleanOptionEntry::ZLSimpleBooleanOptionEntry(const std::string &name, ZLBooleanOption &option) : myName(name), myOption(option) {}
inline ZLSimpleBooleanOptionEntry::~ZLSimpleBooleanOptionEntry() {}
inline const std::string &ZLSimpleBooleanOptionEntry::name() const { return myName; }
inline bool ZLSimpleBooleanOptionEntry::initialState() const { return myOption.value(); }
inline void ZLSimpleBooleanOptionEntry::onAccept(bool state) { myOption.setValue(state); }

inline ZLSpinOptionEntry::ZLSpinOptionEntry() {}
inline ZLSpinOptionEntry::~ZLSpinOptionEntry() {}
inline ZLOptionKind ZLSpinOptionEntry::kind() const { return SPIN; }

inline ZLSimpleSpinOptionEntry::ZLSimpleSpinOptionEntry(const std::string &name, ZLIntegerRangeOption &option, int step) : myName(name), myOption(option), myStep(step) {}
inline ZLSimpleSpinOptionEntry::~ZLSimpleSpinOptionEntry() {}
inline const std::string &ZLSimpleSpinOptionEntry::name() const { return myName; }
inline int ZLSimpleSpinOptionEntry::initialValue() const { return myOption.value(); }
inline int ZLSimpleSpinOptionEntry::minValue() const { return myOption.minValue(); }
inline int ZLSimpleSpinOptionEntry::maxValue() const { return myOption.maxValue(); }
inline int ZLSimpleSpinOptionEntry::step() const { return myStep; }
inline void ZLSimpleSpinOptionEntry::onAccept(int value) { myOption.setValue(value); }

inline ZLComboOptionEntry::ZLComboOptionEntry() {}
inline ZLComboOptionEntry::~ZLComboOptionEntry() {}
inline ZLOptionKind ZLComboOptionEntry::kind() const { return COMBO; }
inline void ZLComboOptionEntry::onValueChange(const std::string&) {}

inline ZLSimpleBoolean3OptionEntry::ZLSimpleBoolean3OptionEntry(const std::string &name, ZLBoolean3Option &option) : myName(name), myOption(option) {}
inline ZLSimpleBoolean3OptionEntry::~ZLSimpleBoolean3OptionEntry() {}
inline const std::string &ZLSimpleBoolean3OptionEntry::name() const { return myName; }

inline ZLColorOptionEntry::ZLColorOptionEntry() {}
inline ZLColorOptionEntry::~ZLColorOptionEntry() {}
inline ZLOptionKind ZLColorOptionEntry::kind() const { return COLOR; }

inline ZLSimpleColorOptionEntry::ZLSimpleColorOptionEntry(ZLColorOption &option) : myOption(option) {}
inline ZLSimpleColorOptionEntry::~ZLSimpleColorOptionEntry() {}
inline const ZLColor ZLSimpleColorOptionEntry::color() const { return myOption.value(); }
inline void ZLSimpleColorOptionEntry::onAccept(ZLColor color) { myOption.setValue(color); }

inline ZLKeyOptionEntry::ZLKeyOptionEntry(const std::string &name) : myName(name) {}
inline ZLKeyOptionEntry::~ZLKeyOptionEntry() {}
inline void ZLKeyOptionEntry::addActionName(const std::string &actionName) { myActionNames.push_back(actionName); }
inline const std::vector<std::string> &ZLKeyOptionEntry::actionNames() const { return myActionNames; }
inline ZLOptionKind ZLKeyOptionEntry::kind() const { return KEY; }
inline const std::string &ZLKeyOptionEntry::name() const { return myName; }

#endif /* __ZLOPTIONENTRY_H__ */
