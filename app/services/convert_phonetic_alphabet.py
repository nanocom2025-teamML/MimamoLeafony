import re

import alkana
from alphabet2kana import a2k
from pykakasi import kakasi


def convert_to_phonetic_alphabet_string_for_atp3012(text: str) -> str:
    """
    引き渡された文章をATP3012の入力データとなる音声記号列に変換する関数
    :param text: 文章
    :return: 音声記号列
    """
    def normalize(text: str) -> str:
        """
        音声記号列を正規化する関数
        :param text: 音声記号列
        :return: 正規化された音声記号列
        """
        normalized_text = re.sub(r'[^a-zA-Z0-9\s\.\?]', ' ', text)
        # 半角スペースとピリオドが連なる場合
        normalized_text = re.sub(r'\s+\.+|\.+\s+', '.', normalized_text)
        # 半角スペースと疑問符が連なる場合
        normalized_text = re.sub(r'\s+\?+|\?+\s+', '?', normalized_text)
        # 疑問符とピリオドが連なる場合
        normalized_text = re.sub(r'\?+\.+|\.+\?+', '?', normalized_text)
        # ピリオドが２つ以上連なる場合
        normalized_text = re.sub(r'\.{2,}', '.', normalized_text)
        # 半角スペースが２つ以上連なる場合
        normalized_text = re.sub(r'\s{2,}', ' ', normalized_text)
        # 疑問符が２つ以上連なる場合
        normalized_text = re.sub(r'\?{2,}', '?', normalized_text)
        # 促音などの調整
        normalized_text = re.sub(r'(tcha|tcya|ctya)', 'ccha', normalized_text)
        normalized_text = re.sub(r'(tchu|tcyu|ctyu)', 'cchu', normalized_text)
        normalized_text = re.sub(r'(tche|tcye|ctye)', 'cche', normalized_text)
        normalized_text = re.sub(r'(tcho|tcyo|ctyo)', 'ccho', normalized_text)
        normalized_text = re.sub(r'(zdya|dzya)', 'ddya', normalized_text)
        normalized_text = re.sub(r'(zdyu|dzyu)', 'ddyu', normalized_text)
        normalized_text = re.sub(r'(zdye|dzye)', 'ddye', normalized_text)
        normalized_text = re.sub(r'(zdyo|dzyo)', 'ddyo', normalized_text)
        # 数字を '<NUMK VAL=***>' で囲う
        normalized_text = re.sub(r'\d+', lambda m: f"<NUMK VAL={m.group(0)}>", normalized_text)

        return normalized_text
    
    kks = kakasi()

    words = [word for word in kks.convert(text)]
    phonetic_alphabets = []

    for word in words:
        if word['orig'] in {',', '，', ' ', '　'}:
            # 読点または空白である場合
            phonetic_alphabets.append(' ')
        elif word['orig'] in {'.', '．', '!', '！'}:
            # 句点又は感嘆符である場合
            phonetic_alphabets.append('.')
        elif word['orig'] in {'?', '？'}:
            # 疑問符である場合
            phonetic_alphabets.append('?')
        elif word['orig'] in {'は'}:
            # 助詞の「は」である場合
            phonetic_alphabets.append('wa')
            phonetic_alphabets.append(' ')
        elif word['orig'] in {'が', 'の', 'を', 'に', 'へと', 'から', 'より', 'で'}:
            # 助詞である場合
            phonetic_alphabets.append(word['hepburn'])
            phonetic_alphabets.append(' ')
        elif word['orig'] == word['hepburn']:
            # 英単語である場合
            for w in word['orig'].split():
                kana = alkana.get_kana(w)
                if kana is None:
                    # 英単語の読みに失敗した場合
                    for _ in kks.convert(a2k(w)):
                        phonetic_alphabets.append(_['hepburn'])
                        phonetic_alphabets.append(' ')
                else:
                    # 英単語の読みに成功した場合
                    for _ in kks.convert(kana):
                        phonetic_alphabets.append(_['hepburn'])
                        phonetic_alphabets.append(' ')
        else:
            phonetic_alphabets.append(word['hepburn'])

    phonetic_alphabets.append('.')

    return normalize("".join(phonetic_alphabets))
