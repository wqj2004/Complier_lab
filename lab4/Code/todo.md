对数组的访问，应该访问数组变量的地址，如
if (op->kind == 数组变量) {
        // 检查是否是数组变量
        int reg = allocate(op, fp);
        fprintf(fp, "  la %s, %s\n", _reg[reg].name, 数组变量的地址名？);38页
        return reg;
    }