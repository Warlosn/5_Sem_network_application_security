#pragma once

#include "string"

std::string GetErrorMsgText(int code);                              // c����������� ����� ������
std::string SetErrorMsgText(const std::string& msgText, int code);
