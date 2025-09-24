.PHONY: rule1 rule2 rule3

var  := "var"
var3 := "var3"
include abcd.mk

var1  = $(var)		#recursive assignment	-> if var changes, var1 changes
var2 := $(var)		#direct assignment		-> if var changes, var2 not changes
var3 ?= $(var)		#if var3 had value, not assign; if var3 not had value, assign recursively

var := "changed"

rule1:
	echo "Hi, I'm rule1"

rule2:
	@echo "Hi, I'm rule2"

rule3:
	@echo "Hi, I'm rule3"
	@echo "$(var1)"
	@echo "$(var2)"
	@echo "$(var3)"