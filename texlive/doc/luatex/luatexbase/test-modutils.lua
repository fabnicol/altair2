-- 
--  This is file `test-modutils.lua',
--  generated with the docstrip utility.
-- 
--  The original source files were:
-- 
--  luatexbase-modutils.dtx  (with options: `testdummy')
--  
--  See the aforementioned source file(s) for copyright and licensing information.
--  
local err, warn, info, log = luatexbase.provides_module {
  name        = 'test-modutils',
  date        = '2000/01/01',
  version     = 1,
  description = 'dummy test package',
}
luatexbase.provides_module {
  name        = 'test-modutils2',
  date        = '',
  version     = 1,
  description = 'dummy test package',
}
info('It works!\nOh, rly?\nYeah rly!')
log("I'm a one-line info.")
info("1 = "..luatexbase.get_module_version('test-modutils'))
if is_module_loaded('test-modutils') then
  info("ok!")
else
  err("problem!")
end
info("2000/01/01 = "..luatexbase.get_module_info('test-modutils').date)
info("20000101 = "..luatexbase.get_module_date_int('test-modutils'))
info("-1 = "..luatexbase.get_module_date_int('test-modutils2'))
-- 
--  End of File `test-modutils.lua'.
