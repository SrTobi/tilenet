#pragma once

#include "Settings.h"


class UnCopyAble
{
protected:
	UnCopyAble() {}
	virtual ~UnCopyAble() {}

private:
	UnCopyAble(const UnCopyAble&);
	UnCopyAble& operator =(const UnCopyAble&);
};