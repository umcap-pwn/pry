# pry — план развития

ELF static analysis tool для Linux. Агрегатор повседневных задач pwn/reverse:
hexdump, ELF-дамп, checksec, дизасм.

## Технические решения

- **Язык:** C11, POSIX.
- **Сборка:** ручной `Makefile` + `config.mk` (стиль suckless/BSD).
  Без autotools и CMake.
- **ELF-парсер:** собственный — ядро проекта.
- **Дизасм:** `libcapstone` (через `pkg-config`), единственная внешняя зависимость.
- **CLI:** один бинарь `pry` с подкомандами (в духе `git`).
- **Вывод:** цвет с самого начала (`isatty`, `NO_COLOR`, `--no-color`).
- **Многопоточность** полностью отсутствует.

## Дерево

```
pry/
├── config.mk              # CC, CFLAGS, CPPFLAGS, LDFLAGS, PREFIX, VERSION
├── Makefile
├── README.md
├── LICENSE
├── docs/ROADMAP.md
├── man/pry.1
├── include/pry/
│   ├── elf.h              # struct Elf, API парсера
│   ├── cmd.h              # прототипы подкоманд + dispatch
│   ├── out.h              # цвет, заголовки, таблицы
│   └── util.h             # xalloc, die, strbuf
├── src/
│   ├── main.c             # entry, глоб. флаги, dispatch
│   ├── elf/               # ядро парсера: только структуры
│   │   ├── elf.c          # open/mmap/validate/close
│   │   ├── header.c       # ehdr + phdr
│   │   ├── sections.c     # shdr + поиск по имени/адресу
│   │   ├── strtab.c
│   │   ├── symbols.c      # symtab/dynsym
│   │   ├── dynamic.c      # NEEDED/RPATH/RUNPATH/SONAME/BIND_NOW
│   │   ├── relocs.c
│   │   └── notes.c        # build-id, ABI, GNU property (CET/IBT/SHSTK)
│   ├── analysis/          # производная логика (не структура ELF)
│   │   └── plt.c          # таблица PLT/GOT из relocs + dynsym
│   ├── cmd/               # тонкие обёртки CLI -> elf/analysis -> out
│   │   ├── hex.c
│   │   ├── elf.c
│   │   ├── strings.c
│   │   ├── checksec.c
│   │   ├── disasm.c
│   │   └── info.c
│   └── util/
│       ├── xalloc.c
│       ├── strbuf.c
│       ├── addr.c         # vaddr <-> file offset
│       └── out.c
├── tests/
│   ├── fixtures/          # генерируются, не коммитятся
│   └── *.sh
└── scripts/gen_fixtures.sh
```

## Подкоманды v1

| команда        | назначение                                                                      |
| -------------- | ------------------------------------------------------------------------------- |
| `pry hex`      | hexdump, перевод vaddr<->offset, `--extract` секции                             |
| `pry elf`      | ehdr/phdr/shdr/symbols/dynamic/relocs/notes, PLT/GOT, interpreter/RPATH/RUNPATH |
| `pry strings`  | строки с оффсетом и секцией                                                     |
| `pry checksec` | NX/PIE/RELRO/canary/fortify/BIND_NOW + strip/static + компилятор                |
| `pry disasm`   | линейный дизасм + листинг функций (capstone)                                    |
| `pry info`     | сводка одним экраном                                                            |

Глобально: `--no-color`, `-h/--help`, `-V/--version`.

## Фазы

### Фаза 0 — каркас

1. `config.mk` — `VERSION`, `PREFIX`, `CC`, `CFLAGS` (`-std=c11 -Wall -Wextra -Wpedantic -O2`), `CPPFLAGS` (`-Iinclude`).
2. `Makefile` — `all/clean/install/uninstall`, объектники из `src/**`.
3. `include/pry/util.h`, `src/util/xalloc.c`, `src/util/strbuf.c`.
4. `include/pry/out.h`, `src/util/out.c` — цвет сразу (`isatty`, `NO_COLOR`, `--no-color`).
5. `include/pry/elf.h`, `src/elf/elf.c` — открыть, `mmap`, проверить `\x7fELF`, `EI_CLASS`, `EI_DATA`, `e_machine`, `e_type`.
6. `include/pry/cmd.h`, `src/main.c` — таблица подкоманд, глоб. флаги, `usage`.
7. `scripts/gen_fixtures.sh` — семплы для проверки каждой фазы.

Итог: `make` собирает `pry --help`.

### Фаза 1 — hex

8. `src/util/addr.c` — карты vaddr<->offset по phdr, поиск секции по адресу.
9. `src/cmd/hex.c` — дамп -> `--extract` -> перевод адресов.

### Фаза 2 — ELF-инфа

10. `elf/{header,sections,strtab,symbols,dynamic,relocs,notes}.c`.
11. `src/cmd/elf.c` — рендер.
12. `src/analysis/plt.c` + таблица.
13. `src/cmd/strings.c`.
14. CET/IBT/SHSTK из `notes.c`.

### Фаза 3 — checksec

15. `src/cmd/checksec.c` — проверки поверх готового парсера.

### Фаза 4 — disasm

16. Подключить capstone в `config.mk`/`Makefile`; внятная ошибка сборки, если нет.
17. `src/cmd/disasm.c` — линейный дизасм, затем листинг функций.

### Фаза 5 — info

18. `src/cmd/info.c` — сводка.

## Вне v1 (следующие волны)

- `cmd/search.c` — поиск байт-паттернов с wildcard'ами.
- `analysis/gadget.c` — ROP/JOP-гаджеты.
- `analysis/xref.c`, `analysis/cfg.c` — перекрёстные ссылки, граф вызовов.
- `cmd/sym.c` — `sym <name>` и `what <addr>`.
- `analysis/libc.c` — версия libc, оффсеты, one_gadget.
- `fmt/json.c` — `--json` ко всем подкомандам.
- генерация pwntools-сниппета, diff двух бинарей.

## Заметки

- **xalloc:** обёртки `xmalloc/xcalloc/xrealloc/xstrdup` с `die()` при OOM.
  Для однофайлового CLI стратегия «упасть и выйти» проще и безопаснее,
  чем восстанавливаться после нехватки памяти.
- **fixtures:** заранее скомпилированные семплы с известными свойствами
  (PIE/no-PIE, canary, RELRO, static/dynamic, stripped, RPATH, CET, 32-bit).
  Генерируются `scripts/gen_fixtures.sh`, в git не хранятся.
  Тесты — shell-скрипты, сверяются с `readelf`/`objdump`/`checksec`.
