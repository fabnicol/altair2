%% To get PSTricks charpath and textpath to work:
%----------- pst-show.pro ------------------------------------------
%!
% Patch for dvipsone to get PSTricks' charpath and textpath to work.
% Save as pst-show.pro, put it with your other PSTricks .pro files.
% Add "\pstheader{pst-show.pro}" to the beginning of your document.
%
% We unbind show:
dvidict begin
/S {show dup 0 rmoveto} def
/T {show 2 index 0 rmoveto} def
/s {show} def
end
%%