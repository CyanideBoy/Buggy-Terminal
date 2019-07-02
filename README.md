# Buggy Terminal
This is a buggy implementation of a terminal done as part of CS347M at IIT Bombay <br>
Standard commands should work on this terminal. The bug is in parallel execution of programs. <br>
<ul>
<li>If a command is followed by &, the command is executed in the background</li>
<li>Multiple user commands separated by && are executed one after the other in sequence in the
foreground </li>
<li>Multiple commands separated by &&& are executed (buggy :P) in parallel in the foreground</li>
<li> SIGINT terminates current foreground process </li>
<li>  <code>exit</code> is to be used for closing the terminal </li>
</ul>
