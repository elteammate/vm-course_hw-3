#include <algorithm>
#include <chrono>
#include <memory>
#include <numeric>
#include <vector>
#include <bitset>
#include <cassert>
#include <cstring>
#include <unordered_map>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using usize = uintptr_t;
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using isize = std::ptrdiff_t;
using f32 = float;
using f64 = double;


#define kase break; case
#define otherwise break; default
#define CONCAT(a, b) a ## b


template <typename T>
std::unique_ptr<T []> pin_array(std::vector<T> vec) {
    std::unique_ptr<T[]> ptr(new T[vec.size()]);
    std::move(vec.begin(), vec.end(), ptr.get());
    return std::move(ptr);
}


namespace el {
#define OP_ENUM_DEFINITION \
        X(BINOP_ADD, 0x01) \
        X(BINOP_SUB, 0x02) \
        X(BINOP_MUL, 0x03) \
        X(BINOP_DIV, 0x04) \
        X(BINOP_REM, 0x05) \
        X(BINOP_LT, 0x06) \
        X(BINOP_LE, 0x07) \
        X(BINOP_GT, 0x08) \
        X(BINOP_GE, 0x09) \
        X(BINOP_EQ, 0x0a) \
        X(BINOP_NE, 0x0b) \
        X(BINOP_AND, 0x0c) \
        X(BINOP_OR, 0x0d) \
        \
        X(CONST, 0x10) \
        X(STRING, 0x11) \
        X(SEXP, 0x12) \
        X(STI, 0x13) \
        X(STA, 0x14) \
        X(JMP, 0x15) \
        X(END, 0x16) \
        X(RET, 0x17) \
        X(DROP, 0x18) \
        X(DUP, 0x19) \
        X(SWAP, 0x1a) \
        X(ELEM, 0x1b) \
        \
        X(LD_GLOBAL, 0x20) \
        X(LD_LOCAL, 0x21) \
        X(LD_ARG, 0x22) \
        X(LD_CLOSURE, 0x23) \
        X(LDA_GLOBAL, 0x30) \
        X(LDA_LOCAL, 0x31) \
        X(LDA_ARG, 0x32) \
        X(LDA_CLOSURE, 0x33) \
        X(ST_GLOBAL, 0x40) \
        X(ST_LOCAL, 0x41) \
        X(ST_ARG, 0x42) \
        X(ST_CLOSURE, 0x43) \
        \
        X(CJMPZ, 0x50) \
        X(CJMPNZ, 0x51) \
        X(BEGIN, 0x52) \
        X(CBEGIN, 0x53) \
        X(CLOSURE, 0x54) \
        X(CALLC, 0x55) \
        X(CALL, 0x56) \
        X(TAG, 0x57) \
        X(FAIL, 0x59) \
        X(LINE, 0x5a) \
        \
        X(PATT_STR, 0x60) \
        X(PATT_ARR, 0x58) \
        X(PATT_STRING, 0x61) \
        X(PATT_ARRAY, 0x62) \
        X(PATT_SEXP, 0x63) \
        X(PATT_REF, 0x64) \
        X(PATT_VAL, 0x65) \
        X(PATT_FUN, 0x66) \
        \
        X(BUILTIN_READ, 0x70) \
        X(BUILTIN_WRITE, 0x71) \
        X(BUILTIN_LENGTH, 0x72) \
        X(BUILTIN_STRING, 0x73) \
        X(BUILTIN_ARRAY, 0x74) \
        \
        X(STOP, 0xff) \

    enum class Op : u8 {
#define X(name, val) name = val,
        OP_ENUM_DEFINITION
#undef X
    };

    bool is_op(u8 byte) {
        switch (byte) {
#define X(name, val) case (u8)Op::name:
        OP_ENUM_DEFINITION
#undef X
            return true;
        default:
            return false;
        }
    }

    struct Meta {
        i32 stage;
        std::vector<std::string> errors;
        std::vector<bool> instruction_valid;
        std::vector<bool> cflow_jump_target;
    };

    struct PublicEntry {
        i32 name_idx;
        i32 offset;
    };

    struct Bytecode {
        void *buffer;
        char *strings;
        usize strings_size;
        PublicEntry *public_table;
        usize public_table_size;
        u8 *code;
        usize code_size;
        usize globals_size;
        Meta meta;

        [[nodiscard]] char *get_string(i32 offset) const {
            if (offset < 0 || offset >= strings_size) [[unlikely]] {
                throw std::runtime_error(std::format("String offset 0x{:08x} is out of bounds", offset));
            }
            return strings + offset;
        }

        [[nodiscard]] Op get_opcode(i32 offset) const {
            if (offset < 0 || offset + 1 > code_size) [[unlikely]] {
                throw std::runtime_error(std::format("Code offset 0x{:08x} is out of bounds", offset));
            }
            if (!is_op(code[offset])) [[unlikely]] {
                throw std::runtime_error(std::format("Invalid opcode 0x{:02x} at offset 0x{:08x}", code[offset], offset));
            }
            return (Op)code[offset];
        }

        [[nodiscard]] u8 get_byte(i32 offset) const {
            if (offset < 0 || offset + 1 > code_size) [[unlikely]] {
                throw std::runtime_error(std::format("Code offset 0x{:08x} is out of bounds", offset));
            }
            return code[offset];
        }

        [[nodiscard]] i32 get_arg(i32 offset) const {
            if (offset < 0 || offset + 4 > code_size) [[unlikely]] {
                throw std::runtime_error(std::format("Code offset 0x{:08x} is out of bounds", offset));
            }
            i32 arg;
            std::memcpy(&arg, code + offset, sizeof(i32));
            return arg;
        }

        [[nodiscard]] char *get_string_arg(i32 offset) const {
            i32 arg = get_arg(offset);
            return get_string(arg);
        }
    } static bc;

    void bytecode_from_bytes(void *buffer, usize size) {
        bc.buffer = buffer;

        struct Header {
            u32 strings_size;
            u32 globals_size;
            u32 public_table_size;
        };

        if (size < sizeof(Header)) {
            throw std::runtime_error("Bytecode is too small");
        }

        Header header = *(Header *)buffer;

        bc.public_table = (PublicEntry *)((u8 *)buffer + sizeof(Header));
        bc.public_table_size = header.public_table_size;
        bc.strings = (char *)((u8 *)bc.public_table + sizeof(PublicEntry) * bc.public_table_size);
        bc.strings_size = header.strings_size;
        bc.code = (u8 *)bc.strings + bc.strings_size;

        if (size < (usize)(bc.code - (u8 *)buffer)) {
            throw std::runtime_error("Code section does not fit in bytecode");
        }

        bc.code_size = size - (usize)(bc.code - (u8 *)buffer);
        bc.globals_size = header.globals_size;
        bc.meta = Meta{
            .stage = 0,
        };
    }

    std::vector<u8> bytecode_from_file(const char *filename) {
        FILE *f;
        if (std::string(filename) == "-") {
            f = stdin;
        } else {
            f = std::fopen(filename, "rb");
            if (!f) {
                throw std::runtime_error("Failed to open file: " + std::string(filename));
            }
        }

        auto close_file = [](FILE *file) { if (file && file != stdin) std::fclose(file); };
        std::unique_ptr<FILE, decltype(close_file)> file_guard(f, close_file);

        if (std::fseek(f, 0, SEEK_END) != 0) {
            throw std::runtime_error("Failed to seek to end of file: " + std::string(filename));
        }

        long file_size = std::ftell(f);
        if (file_size < 0) {
            throw std::runtime_error("Failed to get file size: " + std::string(filename));
        }

        if (std::fseek(f, 0, SEEK_SET) != 0) {
            throw std::runtime_error("Failed to seek to start of file: " + std::string(filename));
        }

        std::vector<u8> bytecode(file_size);
        if (std::fread(bytecode.data(), file_size, 1, f) != 1) {
            throw std::runtime_error("Failed to read file: " + std::string(filename));
        }

        bytecode_from_bytes(bytecode.data(), bytecode.size());
        return bytecode;
    }

    std::variant<i32, std::string> get_instruction_size(i32 offset) {
        if (offset < 0 || offset >= bc.code_size) {
            return std::format("get_instruction_size: Code offset {} out of range", offset);
        }

        u8 opcode = bc.code[offset];
        if (!is_op(opcode)) {
            return std::format("get_instruction_size: Invalid opcode {} at offset {}", opcode, offset);
        }

        switch ((Op)opcode) {
        case Op::BINOP_ADD:
        case Op::BINOP_SUB:
        case Op::BINOP_MUL:
        case Op::BINOP_DIV:
        case Op::BINOP_REM:
        case Op::BINOP_LT:
        case Op::BINOP_LE:
        case Op::BINOP_GT:
        case Op::BINOP_GE:
        case Op::BINOP_EQ:
        case Op::BINOP_NE:
        case Op::BINOP_AND:
        case Op::BINOP_OR:
            return 1;
        case Op::CONST:
        case Op::STRING:
            return 5;
        case Op::SEXP:
            return 9;
        case Op::STI:
        case Op::STA:
            return 1;
        case Op::JMP:
            return 5;
        case Op::END:
        case Op::RET:
        case Op::DROP:
        case Op::DUP:
        case Op::SWAP:
        case Op::ELEM:
            return 1;
        case Op::LD_GLOBAL:
        case Op::LD_LOCAL:
        case Op::LD_ARG:
        case Op::LD_CLOSURE:
        case Op::LDA_GLOBAL:
        case Op::LDA_LOCAL:
        case Op::LDA_ARG:
        case Op::LDA_CLOSURE:
        case Op::ST_GLOBAL:
        case Op::ST_LOCAL:
        case Op::ST_ARG:
        case Op::ST_CLOSURE:
            return 5;
        case Op::CJMPZ:
        case Op::CJMPNZ:
            return 5;
        case Op::BEGIN:
        case Op::CBEGIN:
            return 9;
        case Op::CLOSURE: {
            if (offset + 9 > bc.code_size) {
                return std::format(
                    "get_instruction_size: code of closure instruction must take at least 9 bytes"
                );
            }
            i32 num_captures = bc.get_arg(offset + 5);
            return 9 + 5 * num_captures;
        }
        case Op::CALLC:
            return 5;
        case Op::CALL:
            return 9;
        case Op::TAG:
            return 9;
        case Op::FAIL:
            return 9;
        case Op::LINE:
            return 5;
        case Op::PATT_ARR:
            return 5;
        case Op::PATT_STR:
        case Op::PATT_STRING:
        case Op::PATT_ARRAY:
        case Op::PATT_SEXP:
        case Op::PATT_REF:
        case Op::PATT_VAL:
        case Op::PATT_FUN:
            return 1;
        case Op::BUILTIN_READ:
        case Op::BUILTIN_WRITE:
        case Op::BUILTIN_LENGTH:
        case Op::BUILTIN_STRING:
            return 1;
        case Op::BUILTIN_ARRAY:
            return 5;
        case Op::STOP:
            return 1;
        default:
            throw std::runtime_error("Invalid opcode: this should be unreachable");
        }
    }

    struct FlowInfo {
        bool continues;
        std::optional<i32> jump_target;
        bool returns_after;
    };

    std::variant<FlowInfo, std::string> get_instruction_flow_info(i32 offset) {
        switch (bc.get_opcode(offset)) {
        case Op::JMP: {
            i32 target = bc.get_arg(offset + 1);
            if (target < 0 || target >= bc.code_size) {
                return "Invalid target offset";
            }
            return FlowInfo{
                .continues = false,
                .jump_target = target,
                .returns_after = false,
            };
        }
        kase Op::CJMPZ:
        case Op::CJMPNZ: {
            i32 target = bc.get_arg(offset + 1);
            if (target < 0 || target >= bc.code_size) {
                return "Invalid target offset";
            }
            return FlowInfo{
                .continues = true,
                .jump_target = target,
                .returns_after = false,
            };
        }
        kase Op::END:
        case Op::STOP:
            return FlowInfo{
                .continues = false,
                .jump_target = std::nullopt,
                .returns_after = false,
            };
        kase Op::CALLC:
            return FlowInfo{
                .continues = true,
                .jump_target = std::nullopt,
                .returns_after = true,
            };
        kase Op::CALL: {
            i32 fn = bc.get_arg(offset + 1);
            if (fn < 0 || fn >= bc.code_size) {
                return "Invalid function offset";
            }
            u8 first_opcode = bc.get_byte(fn);
            if (first_opcode != (u8)Op::BEGIN && first_opcode != (u8)Op::CBEGIN) {
                return "Function must start with BEGIN or CBEGIN";
            }
            return FlowInfo{
                .continues = true,
                .jump_target = fn,
                .returns_after = true,
            };
        }
        kase Op::CLOSURE: {
            i32 fn = bc.get_arg(offset + 1);
            if (fn < 0 || fn >= bc.code_size) {
                return "Invalid function offset";
            }
            u8 first_opcode = bc.get_byte(fn);
            if (first_opcode != (u8)Op::BEGIN && first_opcode != (u8)Op::CBEGIN) {
                return "Function must start with BEGIN or CBEGIN";
            }
            return FlowInfo{
                .continues = true,
                .jump_target = fn,
                .returns_after = false,
            };
        }
        default:
            return FlowInfo{
                .continues = true,
                .jump_target = std::nullopt,
                .returns_after = false
            };
        }
    }

    void analyze_bytecode_stage1() {
        bc.meta = Meta{};

        if (bc.code_size == 0) {
            bc.meta.errors.emplace_back("Code section must not be empty");
            return;
        }
        if (auto code = bc.code; code[bc.code_size - 1] != 0xff) {
            bc.meta.errors.emplace_back("Code section must end with 0xff byte");
        }

        bc.meta.instruction_valid.assign(bc.code_size, false);
        bc.meta.cflow_jump_target.assign(bc.code_size, false);
        std::vector visited(bc.code_size, false);

        std::vector<i32> queue;
        queue.reserve(bc.public_table_size + 32);
        for (i32 i = 0; i < bc.public_table_size; i++) {
            i32 fn = bc.public_table[i].offset;
            if (fn < 0 || fn >= bc.code_size) {
                bc.meta.errors.emplace_back(std::format(
                    "Function {} points outside of code segment",
                    bc.get_string(bc.public_table[i].name_idx)
                ));
            } else {
                queue.push_back(fn);
            }
        }

        while (!queue.empty()) {
            i32 offset = queue.back();
            queue.pop_back();

            bc.meta.cflow_jump_target[offset] = true;

            while (true) {
                if (visited[offset]) {
                    break;
                }
                visited[offset] = true;

                auto size_result = get_instruction_size(offset);
                if (std::holds_alternative<std::string>(size_result)) {
                    bc.meta.errors.emplace_back(std::get<std::string>(size_result));
                    break;
                }
                auto size = std::get<i32>(size_result);

                auto flow_info_result = get_instruction_flow_info(offset);
                if (std::holds_alternative<std::string>(flow_info_result)) {
                    bc.meta.errors.emplace_back(std::get<std::string>(flow_info_result));
                    break;
                }
                auto flow_info = std::get<FlowInfo>(flow_info_result);

                bc.meta.instruction_valid[offset] = true;

                if (flow_info.jump_target.has_value()) {
                    queue.push_back(flow_info.jump_target.value());
                }
                offset += size;
                if (offset >= bc.code_size) {
                    break;
                }
                if (flow_info.returns_after) {
                    bc.meta.cflow_jump_target[offset] = true;
                }
                if (!flow_info.continues) break;
            }
        }

        bc.meta.stage = 1;
    }

    void analyze_bytecode() {
        analyze_bytecode_stage1();
    }

    void dump_instruction(FILE *f, i32 offset) {
        if (bc.meta.stage < 1) {
            throw std::runtime_error("Bytecode must be analyzed before formatting");
        }
        Op op = bc.get_opcode(offset);
        i32 arg1_offset = offset + 1;
        i32 arg2_offset = offset + 5;
        switch (op) {
        case Op::BINOP_ADD: std::fprintf(f, "BINOP\t+");
        kase Op::BINOP_SUB: std::fprintf(f, "BINOP\t-");
        kase Op::BINOP_MUL: std::fprintf(f, "BINOP\t*");
        kase Op::BINOP_DIV: std::fprintf(f, "BINOP\t/");
        kase Op::BINOP_REM: std::fprintf(f, "BINOP\t%%");
        kase Op::BINOP_LT: std::fprintf(f, "BINOP\t<");
        kase Op::BINOP_GT: std::fprintf(f, "BINOP\t>");
        kase Op::BINOP_LE: std::fprintf(f, "BINOP\t<=");
        kase Op::BINOP_GE: std::fprintf(f, "BINOP\t>=");
        kase Op::BINOP_EQ: std::fprintf(f, "BINOP\t==");
        kase Op::BINOP_NE: std::fprintf(f, "BINOP\t!=");
        kase Op::BINOP_AND: std::fprintf(f, "BINOP\t&&");
        kase Op::BINOP_OR: std::fprintf(f, "BINOP\t!!");
        kase Op::CONST: std::fprintf(f, "CONST\t%d", bc.get_arg(arg1_offset));
        kase Op::STRING: std::fprintf(f, "STRING\t%s", bc.get_string_arg(arg1_offset));
        kase Op::SEXP: std::fprintf(f, "SEXP\t%s %d", bc.get_string_arg(arg1_offset), bc.get_arg(arg2_offset));
        kase Op::STI: std::fprintf(f, "STI");
        kase Op::STA: std::fprintf(f, "STA");
        kase Op::JMP: std::fprintf(f, "JMP\t0x%.8x", bc.get_arg(arg1_offset));
        kase Op::END: std::fprintf(f, "END");
        kase Op::RET: std::fprintf(f, "RET");
        kase Op::DROP: std::fprintf(f, "DROP");
        kase Op::DUP: std::fprintf(f, "DUP");
        kase Op::SWAP: std::fprintf(f, "SWAP");
        kase Op::ELEM: std::fprintf(f, "ELEM");
        kase Op::LD_GLOBAL: std::fprintf(f, "LD\tG(%d)", bc.get_arg(arg1_offset));
        kase Op::LD_LOCAL: std::fprintf(f, "LD\tL(%d)", bc.get_arg(arg1_offset));
        kase Op::LD_ARG: std::fprintf(f, "LD\tA(%d)", bc.get_arg(arg1_offset));
        kase Op::LD_CLOSURE: std::fprintf(f, "LD\tC(%d)", bc.get_arg(arg1_offset));
        kase Op::LDA_GLOBAL: std::fprintf(f, "LDA\tG(%d)", bc.get_arg(arg1_offset));
        kase Op::LDA_LOCAL: std::fprintf(f, "LDA\tL(%d)", bc.get_arg(arg1_offset));
        kase Op::LDA_ARG: std::fprintf(f, "LDA\tA(%d)", bc.get_arg(arg1_offset));
        kase Op::LDA_CLOSURE: std::fprintf(f, "LDA\tC(%d)", bc.get_arg(arg1_offset));
        kase Op::ST_GLOBAL: std::fprintf(f, "ST\tG(%d)", bc.get_arg(arg1_offset));
        kase Op::ST_LOCAL: std::fprintf(f, "ST\tL(%d)", bc.get_arg(arg1_offset));
        kase Op::ST_ARG: std::fprintf(f, "ST\tA(%d)", bc.get_arg(arg1_offset));
        kase Op::ST_CLOSURE: std::fprintf(f, "ST\tC(%d)", bc.get_arg(arg1_offset));
        kase Op::CJMPZ: std::fprintf(f, "CJMPz\t0x%.8x", bc.get_arg(arg1_offset));
        kase Op::CJMPNZ: std::fprintf(f, "CJMPnz\t0x%.8x", bc.get_arg(arg1_offset));
        kase Op::BEGIN: {
            i32 args = bc.get_arg(arg1_offset);
            i32 locals = bc.get_arg(arg2_offset);
            std::fprintf(f, "BEGIN\t%d %d", args, locals);
        }
        kase Op::CBEGIN: {
            i32 args = bc.get_arg(arg1_offset);
            i32 locals = bc.get_arg(arg2_offset);
            std::fprintf(f, "CBEGIN\t%d %d", args, locals);
        }
        kase Op::CLOSURE: {
            std::fprintf(f, "CLOSURE\t0x%.8x", bc.get_arg(arg1_offset));
            i32 num_captures = bc.get_arg(arg2_offset);
            for (i32 i = 0, designator_offset = offset + 9; i < num_captures; i++, designator_offset += 5) {
                if (u8 designator = bc.get_byte(designator_offset); designator == 0) {
                    std::fprintf(f, "G(%d)", bc.get_arg(designator_offset + 1));
                } else if (designator == 1) {
                    std::fprintf(f, "L(%d)", bc.get_arg(designator_offset + 1));
                } else if (designator == 2) {
                    std::fprintf(f, "A(%d)", bc.get_arg(designator_offset + 1));
                } else if (designator == 3) {
                    std::fprintf(f, "C(%d)", bc.get_arg(designator_offset + 1));
                } else {
                    throw std::runtime_error(std::format("Invalid capture designator %d", num_captures));
                }
            }
        }
        kase Op::CALLC: std::fprintf(f, "CALLC\t%d", bc.get_arg(arg1_offset));
        kase Op::CALL: {
            i32 fn = bc.get_arg(arg1_offset);
            i32 num_args = bc.get_arg(arg2_offset);
            std::fprintf(f, "CALL\t0x%.8x %d", fn, num_args);
        }
        kase Op::TAG: {
            char *s = bc.get_string_arg(arg1_offset);
            i32 num = bc.get_arg(arg2_offset);
            std::fprintf(f, "TAG\t%s %d", s, num);
        }
        kase Op::FAIL: {
            i32 line = bc.get_arg(arg1_offset);
            i32 column = bc.get_arg(arg2_offset);
            std::fprintf(f, "FAIL\t%d %d", line, column);
        }
        kase Op::LINE: std::fprintf(f, "LINE\t%d", bc.get_arg(arg1_offset));
        kase Op::PATT_STR: std::fprintf(f, "PATT\t=str");
        kase Op::PATT_STRING: std::fprintf(f, "PATT\t#string");
        kase Op::PATT_ARRAY: std::fprintf(f, "PATT\t#array");
        kase Op::PATT_ARR: std::fprintf(f, "ARRAY\t%d", bc.get_arg(arg1_offset));
        kase Op::PATT_SEXP: std::fprintf(f, "PATT\t#sexp");
        kase Op::PATT_REF: std::fprintf(f, "PATT\t#ref");
        kase Op::PATT_VAL: std::fprintf(f, "PATT\t#val");
        kase Op::PATT_FUN: std::fprintf(f, "PATT\t#fun");
        kase Op::BUILTIN_READ: std::fprintf(f, "CALL\tLread");
        kase Op::BUILTIN_WRITE: std::fprintf(f, "CALL\tLwrite");
        kase Op::BUILTIN_LENGTH: std::fprintf(f, "CALL\tLlength");
        kase Op::BUILTIN_STRING: std::fprintf(f, "CALL\tLstring");
        kase Op::BUILTIN_ARRAY: std::fprintf(f, "CALL\tBarray\t%d", bc.get_arg(arg1_offset));
        kase Op::STOP: std::fprintf(f, "<end>");
        }
    }

    void dump_bytecode(FILE *f) {
        if (bc.meta.stage < 1) {
            throw std::runtime_error("Bytecode must be analyzed before formatting");
        }

        std::fprintf(f, "String table size       : %lu\n", bc.strings_size);
        std::fprintf(f, "Global area size        : %lu\n", bc.globals_size);
        std::fprintf(f, "Number of public symbols: %lu\n", bc.public_table_size);
        std::fprintf(f, "Public symbols          :\n");

        for (usize i = 0; i < bc.public_table_size; i++) {
            auto [name_offset, offset] = bc.public_table[i];
            std::fprintf(f, "   0x%.8x: %s\n", offset, bc.get_string(name_offset));
        }
        std::fprintf(f, "Code:\n");

        for (i32 offset = 0; offset < bc.code_size; offset++) {
            if (!bc.meta.instruction_valid[offset]) {
                continue;
            }
            std::fprintf(f, "0x%.8x:\t", offset);
            dump_instruction(f, offset);
            std::fprintf(f, "\n");
        }
    }
}


struct CodeRange {
    u32 offset : 31;
    bool binary : 1;

    [[nodiscard]] std::pair<u8 *, i32> locate() const {
        using el::bc;
        i32 size1 = std::get<i32>(el::get_instruction_size((i32)this->offset));
        if (this->binary) {
            i32 size2 = std::get<i32>(el::get_instruction_size((i32)this->offset + size1));
            return {bc.code + this->offset, size1 + size2};
        }
        return {bc.code + this->offset, size1};
    }

    std::strong_ordering operator<=>(const CodeRange &other) const {
        using el::bc;
        if (this->binary != other.binary) return this->binary <=> other.binary;
        auto [this_code, this_size] = this->locate();
        auto [other_code, other_size] = other.locate();
        if (this_size != other_size) return this_size <=> other_size;
        return std::memcmp(this_code, other_code, this_size) <=> 0;
    }

    bool operator==(const CodeRange &other) const {
        return *this <=> other == std::strong_ordering::equal;
    }
};

struct CodeRangeHasher {
    size_t operator()(const CodeRange &i) const {
        auto [code, size] = i.locate();
        return std::hash<std::string_view>{}({(char *)code, (size_t)size});
    }
};

void count_idioms() {
    using el::bc;

    assert(bc.meta.stage >= 1);

    std::vector<CodeRange> idioms;
    idioms.reserve(bc.code_size * 2);

    for (i32 offset = 0; offset < bc.code_size; offset++) {
        if (!bc.meta.instruction_valid[offset]) {
            continue;
        }
        i32 size = std::get<i32>(el::get_instruction_size(offset));
        CodeRange inst{
            .offset = (u32)offset,
            .binary = false,
        };
        idioms.push_back(inst);

        if (bc.meta.instruction_valid[offset + size] && !bc.meta.cflow_jump_target[offset + size]) {
            CodeRange bin_inst{
                .offset = (u32)offset,
                .binary = true,
            };
            idioms.push_back(bin_inst);
        }
    }

    std::ranges::sort(idioms);

    std::vector<std::pair<CodeRange, i32>> counts;

    counts.emplace_back(idioms[0], 1);

    for (usize i = 1; i < idioms.size(); i++) {
        if (idioms[i] == counts.back().first) {
            counts.back().second++;
        } else {
            counts.emplace_back(idioms[i], 1);
        }
    }

    std::ranges::sort(counts, [](const auto &a, const auto &b) {
        return a.second > b.second;
    });

    std::printf("Most common idioms:\n");
    std::printf("Rank Occurs\tIdiom\n");
    for (i32 i = 0; i < counts.size(); ++i) {
        const auto &[inst, count] = counts[i];
        std::printf("%4d:", i + 1);
        std::printf("%6d\t", count);
        el::dump_instruction(stdout, (i32)inst.offset);
        if (inst.binary) {
            std::printf("\n");
            std::printf("%11s\t", "");
            i32 size = std::get<i32>(el::get_instruction_size((i32)inst.offset));
            el::dump_instruction(stdout, (i32)inst.offset + size);
        }
        std::printf("\n");
    }
}


i32 app_decompile(const char *bytecode_filename) {
    using el::bc;

    auto guard = el::bytecode_from_file(bytecode_filename);
    el::analyze_bytecode_stage1();
    if (!bc.meta.errors.empty()) {
        for (const auto& err : bc.meta.errors) {
            std::fprintf(stderr, "Bytecode validation error: %s\n", err.c_str());
        }
        return 1;
    }
    el::dump_bytecode(stdout);
    return 0;
}

i32 app_count_idioms(const char *bytecode_filename) {
    using el::bc;

    auto guard = el::bytecode_from_file(bytecode_filename);
    el::analyze_bytecode();
    if (!bc.meta.errors.empty()) {
        for (const auto& err : bc.meta.errors) {
            std::fprintf(stderr, "Bytecode validation error: %s\n", err.c_str());
        }
        return 1;
    }

    std::printf("Bytecode:\n");
    el::dump_bytecode(stdout);
    std::printf("\n");
    count_idioms();

    return 0;
}

i32 main(i32 argc, char *argv[]) try {
    if (argc < 2) {
        std::printf("Usage: %s <bytecode.bc>\n", argv[0]);
        return 1;
    }

    return app_count_idioms(argv[1]);
} catch (const std::runtime_error &e) {
    std::printf("%s\n", e.what());
    return 1;
}
