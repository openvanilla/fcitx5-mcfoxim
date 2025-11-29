import json
import xlrd
import os

langs = {
    "00": {"en": "Nanshi Amis", "zh_Tw": "南勢阿美語", "iso639_3": "ami"},
    "01": {"en": "Siwkolan Amis", "zh_Tw": "秀姑巒阿美語", "iso639_3": "ami"},
    "02": {"en": "Coastal Amis", "zh_Tw": "海岸阿美語", "iso639_3": "ami"},
    "03": {"en": "Malan Amis", "zh_Tw": "馬蘭阿美語", "iso639_3": "ami"},
    "04": {"en": "Hengchun Amis", "zh_Tw": "恆春阿美語", "iso639_3": "ami"},

    "05": {"en": "Squliq Atayal", "zh_Tw": "賽考利克泰雅語", "iso639_3": "tay"},
    "06": {"en": "C'uli' Atayal", "zh_Tw": "澤敖利泰雅語", "iso639_3": "tay"},
    "07": {"en": "Wenshui Atayal", "zh_Tw": "汶水泰雅語", "iso639_3": "tay"},
    "08": {"en": "Wanta Atayal", "zh_Tw": "萬大泰雅語", "iso639_3": "tay"},
    "09": {"en": "Sijie Atayal", "zh_Tw": "四季泰雅語", "iso639_3": "tay"},
    "10": {"en": "Yilan C'uli' Atayal", "zh_Tw": "宜蘭澤敖利泰雅語", "iso639_3": "tay"},

    "12": {"en": "Saisiyat", "zh_Tw": "賽夏語", "iso639_3": "xsy"},
    "13": {"en": "Thao", "zh_Tw": "邵語", "iso639_3": "ssf"},

    "14": {"en": "Toda Seediq", "zh_Tw": "都達賽德克語", "iso639_3": "trv"},
    "15": {"en": "Tgdaya Seediq", "zh_Tw": "德固達雅賽德克語", "iso639_3": "trv"},
    "16": {"en": "Truku (Taroko) Seediq", "zh_Tw": "德鹿谷賽德克語", "iso639_3": "trv"},

    "17": {"en": "Takbanuaz Bunun", "zh_Tw": "卓群布農語", "iso639_3": "bnn"},
    "18": {"en": "Takitudu Bunun", "zh_Tw": "卡群布農語", "iso639_3": "bnn"},
    "19": {"en": "Isbukun Bunun", "zh_Tw": "丹群布農語", "iso639_3": "bnn"},
    "20": {"en": "Bun-an Bunun", "zh_Tw": "巒群布農語", "iso639_3": "bnn"},
    "21": {"en": "Takivatan Bunun", "zh_Tw": "郡群布農語", "iso639_3": "bnn"},

    "22": {"en": "Eastern Paiwan", "zh_Tw": "東排灣語", "iso639_3": "pwn"},
    "23": {"en": "Northern Paiwan", "zh_Tw": "北排灣語", "iso639_3": "pwn"},
    "24": {"en": "Central Paiwan", "zh_Tw": "中排灣語", "iso639_3": "pwn"},
    "25": {"en": "Southern Paiwan", "zh_Tw": "南排灣語", "iso639_3": "pwn"},

    "26": {"en": "Eastern Rukai", "zh_Tw": "東魯凱語", "iso639_3": "dru"},
    "27": {"en": "Wutai Rukai", "zh_Tw": "霧台魯凱語", "iso639_3": "dru"},
    "28": {"en": "Dawu Rukai", "zh_Tw": "大武魯凱語", "iso639_3": "dru"},
    "29": {"en": "Tona Rukai", "zh_Tw": "多納魯凱語", "iso639_3": "dru"},
    "30": {"en": "Maolin Rukai", "zh_Tw": "茂林魯凱語", "iso639_3": "dru"},
    "31": {"en": "Wanshan Rukai", "zh_Tw": "萬山魯凱語", "iso639_3": "dru"},

    "32": {"en": "Taroko", "zh_Tw": "太魯閣語", "iso639_3": "trv"},
    "33": {"en": "Kavalan", "zh_Tw": "噶瑪蘭語", "iso639_3": "ckv"},
    "34": {"en": "Tsou", "zh_Tw": "鄒語", "iso639_3": "tsu"},
    "35": {"en": "Kanakanavu", "zh_Tw": "卡那卡那富語", "iso639_3": "xnb"},
    "36": {"en": "Saaroa", "zh_Tw": "拉阿魯哇語", "iso639_3": "sxr"},

    "37": {"en": "Nanwang Puyuma", "zh_Tw": "南王卑南語", "iso639_3": "pyu"},
    "38": {"en": "Tjipen Puyuma", "zh_Tw": "知本卑南語", "iso639_3": "pyu"},
    "39": {"en": "Western Puyuma", "zh_Tw": "西群卑南語", "iso639_3": "pyu"},
    "40": {"en": "Jianhe Puyuma", "zh_Tw": "建和卑南語", "iso639_3": "pyu"},

    "41": {"en": "Tao (Yami)", "zh_Tw": "雅美語", "iso639_3": "tao"},
    "42": {"en": "Sakizaya", "zh_Tw": "撒奇萊雅語", "iso639_3": "szy"}
}



def parse_excel_file(path):
    workbook = xlrd.open_workbook(path)
    sheet = workbook.sheet_by_index(0)
    name = sheet.cell_value(0, 2)
    data = []
    for row_idx in range(3, sheet.nrows):
        row = sheet.row(row_idx)
        ch = row[2].value
        text = row[3].value
        if "無此詞彙" in text:
            continue
        if ch and text:
            if "/" in text:
                components = text.split("/")
                for component in components:
                    data.append((component.strip(), ch))
            else:
                data.append((text.strip(), ch))
    data.sort(key=lambda x: x[0])
    table = {"name": name, "data": data}
    return table


def main():
    """
    Converts Excel files in the current directory to TypeScript files.
    """
    for i in range(0, 43):
        current_folder = os.getcwd()
        current_folder = os.path.join(current_folder, "glossary")
        print(current_folder)
        files = os.listdir(current_folder)
        path = next((f for f in files if "-{:02d}".format(i + 1) in f), None)
        print(path)
        try:
            print(current_folder)
            print(path)
            file_path = os.path.join(current_folder, path)
            data = parse_excel_file(file_path)
            index = "{:02d}".format(i)
            name = "TW_" + index
            write_file = os.path.join("../data", name + ".json")
            with open(write_file, "w") as json_file:
                json.dump(data, json_file, ensure_ascii=False, indent=2)

            conf_folder = os.path.join("../src")
            conf_file = os.path.join(conf_folder, "fox_"+ name + ".conf")
            with open(conf_file, "w") as f:
                f.write("[InputMethod]\n")
                f.write(f"Name=McFoxIM - {langs[index]['en']}\n") 
                f.write(f"Name[en]=McFoxIM - {langs[index]['en']}\n") 
                f.write(f"Name[zh_Tw]=小麥族語 - {langs[index]['zh_Tw']}\n") 
                f.write("Icon=fcitx_mcfoxim\n")
                f.write(f"Label={langs[index]['zh_Tw']}\n")
                f.write(f"LangCode={langs[index]['iso639_3']}\n")
                f.write("Addon=fox\n")

        except Exception as e:
            print("Error processing {}: {}".format(path, e))
            # Generate conf file for each processed Excel file


if __name__ == "__main__":
    main()
    print("Data processed and written to files")
