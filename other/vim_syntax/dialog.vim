" Vim syntax file
" Language: DNT Dialogs Language
" Maintainer: Guilherme Farrer <farrer@dnteam.org>
" Last Change: 2012 Jul 22 

syn clear

" Dialog
syn keyword dntDialog         dialog end_dialog

" Blocks
syn keyword dntBlock          npc_begin npc_end
syn keyword dntBlock          pc_begin pc_end
syn keyword dntBlock          option

" Fields
syn keyword dntField          text
syn keyword dntField          action if_action else_action
syn keyword dntField          pre_test post_test

" String
syn region dntString          start=+"+   end=+"+
syn match dntNumber           "-\=\<\d\+\>"
syn match dntByte             "\$[0-9a-fA-F]\+\>"

" Comments
syn match   dntComment        "#.*$" contains=dntTodo
syn keyword dntTodo           contained TODO FIXME XXX NOTE

" Gettext
syn keyword dntGettext        gettext

" Functions
syn keyword dntFunction       go_to_dialog dialog_init finish_dialog
syn keyword dntFunction       init_fight
syn keyword dntFunction       add_mission complete_mission
syn keyword dntFunction       give_item receive_item give_money receive_money
syn keyword dntFunction       receive_xp change_object_state
syn keyword dntFunction       kill_all map_travel
syn keyword dntFunction       call_script
syn keyword dntFunction       open play_sound

" Tests
syn keyword dntTest           roll
syn keyword dntTest           greater lesser equal diff 
syn keyword dntTest           align align_not
syn keyword dntTest           have_item have_item_with_info
syn keyword dntTest           all_dead all_alive have_money
syn keyword dntTest           mission_active

syn sync lines=250

if !exists("did_dialog_syntax_inits")
   let did_dialog_syntax_inits = 1
   " The default methods for highlighting.  Can be overridden later
   hi link dntBlock           Statement
   hi link dntString          String
   hi link dntNumber          Number
   hi link dntByte            Number
   hi link dntComment         Comment
   hi link dntTodo            Todo
   hi link dntFunction        Function
   hi link dntDialog          Keyword
   hi link dntTest            PreProc
   hi link dntGettext         Identifier
   hi link dntField           Special
endif


let b:current_syntax = "dialog"

