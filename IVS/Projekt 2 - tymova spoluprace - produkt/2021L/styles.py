"""
author:
    Samuel Kuchta (xkucht11@stud.fit.vutbr.cz)
file:
    styles.py
brief:
    Class for changing GUI color.
"""


class Palette:
    """Manages color palettes of main window"""
    # main background colors
    win_style = "background-color:  rgb(000, 255, 000);\n" \
                "color:  rgb(255, 000, 255);"

    # style used by numeric buttons
    num_but_style = "QPushButton{\n" \
                    "background-color: rgb(255, 255, 255);\n" \
                    "border-style: solid;\n" \
                    "border-width: 2px;\n" \
                    "border-radius: 10px;\n" \
                    "border-color: rgb(000, 000, 000);\n" \
                    "color: rgb(000, 000, 000);\n" \
                    "font: bold 18px;\n" \
                    "min-width: 1.5em;\n" \
                    "min-height: 1.5em;\n" \
                    "padding: 6px;\n" \
                    "}\n" \
                    "QPushButton:pressed {\n" \
                    "background-color: rgb(200, 200, 200);\n" \
                    "border-style: inset;\n" \
                    "}"

    # style used by control buttons
    ctrl_but_style = "QPushButton{\n" \
                     "background-color: rgb(255, 255, 255);\n" \
                     "border-style: solid;\n" \
                     "border-width: 2px;\n" \
                     "border-radius: 10px;\n" \
                     "border-color: rgb(255, 000, 000);\n" \
                     "color: rgb(255, 000, 000);\n" \
                     "font: bold 18px;\n" \
                     "min-width: 1.5em;\n" \
                     "min-height: 1.5em;\n" \
                     "padding: 6px;\n" \
                     "}\n" \
                     "QPushButton:pressed {\n" \
                     "background-color: rgb(200, 200, 200);\n" \
                     "border-style: inset;\n" \
                     "}"

    # style used by operator buttons
    op_but_style = "QPushButton{\n" \
                   "background-color: rgb(255, 255, 255);\n" \
                   "border-style: solid;\n" \
                   "border-width: 2px;\n" \
                   "border-radius: 10px;\n" \
                   "border-color: rgb(000, 000, 255);\n" \
                   "color: rgb(000, 000, 255);\n" \
                   "font: bold 18px;\n" \
                   "min-width: 1.5em;\n" \
                   "min-height: 1.5em;\n" \
                   "padding: 6px;\n" \
                   "}\n" \
                   "QPushButton:pressed {\n" \
                   "background-color: rgb(200, 200, 200);\n" \
                   "border-style: inset;\n" \
                   "}"

    # style used by display
    display_style = "QLineEdit{\n" \
                    "background-color: rgb(255, 255, 255);\n" \
                    "border-style: solid;\n" \
                    "border-width: 2px;\n" \
                    "border-radius: 10px;\n" \
                    "border-color: rgb(000, 000, 000);\n" \
                    "color: rgb(000, 000, 000);\n" \
                    "font: bold 18px;\n" \
                    "min-width: 1.5em;\n" \
                    "min-height: 1.5em;\n" \
                    "padding: 6px;\n" \
                    "text-align: right;\n" \
                    "}"

    # style used by history
    hist_style = "QLineEdit{\n" \
                 "background-color: rgb(255, 255, 255);\n" \
                 "border-style: solid;\n" \
                 "border-width: 1px;\n" \
                 "border-radius: 10px;\n" \
                 "border-color: rgb(000, 000, 000);\n" \
                 "color: rgb(000, 000, 000);\n" \
                 "font: bold 18px;\n" \
                 "min-width: 1em;\n" \
                 "min-height: 1em;\n" \
                 "text-align: right;\n" \
                 "}"

    colors = [  # color palettes
        [  # window
            [  # bg
                "240, 240, 240",  # 0. white
                "050, 050, 050",  # 1. dark-grey
                "000, 032, 063",  # 2. dark-blue
                "052, 049, 072",  # 3. dark-blue
                "075, 135, 139",  # 4. blue
                "000, 177, 210",  # 5. blue
                "000, 107, 056",  # 6. green
                "044, 095, 045",  # 7. green
                "043, 174, 102",  # 8. green
                "233, 075, 060",  # 9. red
                "016, 024, 032",  # 10. dark-cyan
                "000, 083, 156",  # 11. blue
                "249, 087, 000",  # 12. orange
                "028, 028, 027",  # 13. black
                "007, 130, 130",  # 14. cyan
                "110, 076, 030",  # 15. dark-brown
                "158, 217, 204",  # 16. blue
                "048, 023, 040",  # 17. dark-violet
                "046, 082, 102",  # 18. dark-blue
                "223, 220, 229",  # 19. grey
                "001, 025, 054"  # 20. dark-blue

            ],
            [  # text
                "000, 000, 000",  # 0. black
                "255, 255, 255",  # 1. white
                "255, 255, 255",  # 2. white
                "255, 255, 255",  # 3. white
                "255, 255, 255",  # 4. TODO
                "000, 000, 000",  # 5. black
                "255, 255, 255",  # 6. TODO
                "255, 255, 255",  # 7. TODO
                "000, 000, 000",  # 8. black
                "000, 000, 000",  # 9. black
                "255, 255, 255",  # 10. TODO
                "255, 255, 255",  # 11. TODO
                "255, 255, 255",  # 12. TODO
                "255, 255, 255",  # 13. TODO
                "255, 255, 255",  # 14. TODO
                "255, 255, 255",  # 15. TODO
                "000, 000, 000",  # 16. black
                "255, 255, 255",  # 17. TODO
                "255, 255, 255",  # 18. TODO
                "000, 000, 000",  # 19. black
                "255, 255, 255"  # 20. TODO
            ]
        ],
        [  # numeric
            [  # bg
                "255, 255, 255",  # 0. white
                "015, 015, 015",  # 1. dark-grey
                "015, 015, 015",  # 2. TODO
                "015, 015, 015",  # 3. TODO
                "015, 015, 015",  # 4. TODO
                "015, 015, 015",  # 5. TODO
                "015, 015, 015",  # 6. TODO
                "015, 015, 015",  # 7. TODO
                "015, 015, 015",  # 8. TODO
                "015, 015, 015",  # 9. TODO
                "015, 015, 015",  # 10. TODO
                "015, 015, 015",  # 11. TODO
                "015, 015, 015",  # 12. TODO
                "015, 015, 015",  # 13. TODO
                "015, 015, 015",  # 14. TODO
                "015, 015, 015",  # 15. TODO
                "252, 246, 245",  # 16. white
                "015, 015, 015",  # 17. TODO
                "015, 015, 015",  # 18. TODO
                "015, 015, 015",  # 19. TODO
                "015, 015, 015"  # 20. TODO
            ],
            [  # border
                "000, 000, 000",  # 0. black
                "000, 000, 000",  # 1. black
                "173, 239, 209",  # 2. light-blue
                "215, 196, 158",  # 3. grey
                "249, 087, 000",  # 4. orange
                "253, 219, 039",  # 5. yellow
                "016, 024, 032",  # 6. black
                "151, 188, 098",  # 7. light-green
                "252, 246, 245",  # 8. white
                "045, 041, 038",  # 9. black
                "254, 231, 021",  # 10. yellow
                "255, 214, 098",  # 11. orange
                "255, 255, 255",  # 12. white
                "206, 074, 126",  # 13. pink
                "051, 158, 102",  # 14. green
                "255, 214, 083",  # 15. gold
                "000, 140, 118",  # 16. cyan
                "079, 052, 102",  # 17. dark-purple
                "110, 136, 152",  # 18. blue
                "171, 209, 201",  # 19. light-blue
                "244, 255, 253"  # 20. white

            ],
            [  # text
                "000, 000, 000",  # 0. black
                "255, 255, 255",  # 1. white
                "255, 255, 255",  # 2. TODO
                "255, 255, 255",  # 3. TODO
                "255, 255, 255",  # 4. TODO
                "255, 255, 255",  # 5. TODO
                "255, 255, 255",  # 6. TODO
                "255, 255, 255",  # 7. TODO
                "255, 255, 255",  # 8. TODO
                "255, 255, 255",  # 9. TODO
                "255, 255, 255",  # 10. TODO
                "255, 255, 255",  # 11. TODO
                "255, 255, 255",  # 12. TODO
                "255, 255, 255",  # 13. TODO
                "255, 255, 255",  # 14. TODO
                "255, 214, 083",  # 15. gold
                "000, 140, 118",  # 16. cyan
                "079, 052, 102",  # 17. dark-purple
                "110, 136, 152",  # 18. blue
                "171, 209, 201",  # 19. light-blue
                "244, 255, 253"  # 20. white
            ],
            [  # pressed bg
                "200, 200, 200",  # 0. light-grey
                "030, 030, 030",  # 1. dark-grey
                "030, 030, 030",  # 2. TODO
                "030, 030, 030",  # 3. TODO
                "030, 030, 030",  # 4. TODO
                "030, 030, 030",  # 5. TODO
                "030, 030, 030",  # 6. TODO
                "030, 030, 030",  # 7. TODO
                "030, 030, 030",  # 8. TODO
                "030, 030, 030",  # 9. TODO
                "030, 030, 030",  # 10. TODO
                "030, 030, 030",  # 11. TODO
                "030, 030, 030",  # 12. TODO
                "030, 030, 030",  # 13. TODO
                "030, 030, 030",  # 14. TODO
                "030, 030, 030",  # 15. TODO
                "030, 030, 030",  # 16. TODO
                "030, 030, 030",  # 17. TODO
                "030, 030, 030",  # 18. TODO
                "030, 030, 030",  # 19. TODO
                "030, 030, 030"  # 20. TODO
            ]
        ],
        [  # control
            [  # bg
                "255, 255, 255",  # 0. white
                "025, 025, 025",  # 1. dark-grey
                "025, 025, 025",  # 2. TODO
                "025, 025, 025",  # 3. TODO
                "025, 025, 025",  # 4. TODO
                "025, 025, 025",  # 5. TODO
                "025, 025, 025",  # 6. TODO
                "025, 025, 025",  # 7. TODO
                "025, 025, 025",  # 8. TODO
                "025, 025, 025",  # 9. TODO
                "025, 025, 025",  # 10. TODO
                "025, 025, 025",  # 11. TODO
                "025, 025, 025",  # 12. TODO
                "025, 025, 025",  # 13. TODO
                "025, 025, 025",  # 14. TODO
                "025, 025, 025",  # 15. TODO
                "025, 025, 025",  # 16. TODO
                "025, 025, 025",  # 17. TODO
                "025, 025, 025",  # 18. TODO
                "025, 025, 025",  # 19. TODO
                "025, 025, 025"  # 20. TODO
            ],
            [  # border
                "255, 000, 000",  # 0. red
                "255, 000, 000",  # 1. red
                "255, 000, 000",  # 2. TODO
                "255, 000, 000",  # 3. TODO
                "255, 000, 000",  # 4. TODO
                "255, 000, 000",  # 5. TODO
                "255, 000, 000",  # 6. TODO
                "255, 000, 000",  # 7. TODO
                "255, 000, 000",  # 8. TODO
                "255, 000, 000",  # 9. TODO
                "255, 000, 000",  # 10. TODO
                "255, 000, 000",  # 11. TODO
                "255, 000, 000",  # 12. TODO
                "255, 000, 000",  # 13. TODO
                "255, 000, 000",  # 14. TODO
                "221, 182, 093",  # 15. brown
                "250, 160, 148",  # 16. dark-pink
                "159, 107, 153",  # 17. violet
                "159, 177, 188",  # 18. light-blue
                "151, 179, 208",  # 19. blue
                "249, 220, 092"  # 20. yellow
            ],
            [  # text
                "255, 000, 000",  # 0. red
                "255, 000, 000",  # 1. red
                "255, 000, 000",  # 2. TODO
                "255, 000, 000",  # 3. TODO
                "255, 000, 000",  # 4. TODO
                "255, 000, 000",  # 5. TODO
                "255, 000, 000",  # 6. TODO
                "255, 000, 000",  # 7. TODO
                "255, 000, 000",  # 8. TODO
                "255, 000, 000",  # 9. TODO
                "255, 000, 000",  # 10. TODO
                "255, 000, 000",  # 11. TODO
                "255, 000, 000",  # 12. TODO
                "255, 000, 000",  # 13. TODO
                "255, 000, 000",  # 14. TODO
                "221, 182, 093",  # 15. brown
                "250, 160, 148",  # 16. dark-pink
                "159, 107, 153",  # 17. violet
                "159, 177, 188",  # 18. light-blue
                "151, 179, 208",  # 19. blue
                "249, 220, 092"  # 20. yellow
            ],
            [  # pressed bg
                "200, 200, 200",  # 0. light-grey
                "030, 030, 030",  # 1. dark-grey
                "030, 030, 030",  # 2. TODO
                "030, 030, 030",  # 3. TODO
                "030, 030, 030",  # 4. TODO
                "030, 030, 030",  # 5. TODO
                "030, 030, 030",  # 6. TODO
                "030, 030, 030",  # 7. TODO
                "030, 030, 030",  # 8. TODO
                "030, 030, 030",  # 9. TODO
                "030, 030, 030",  # 10. TODO
                "030, 030, 030",  # 11. TODO
                "030, 030, 030",  # 12. TODO
                "030, 030, 030",  # 13. TODO
                "030, 030, 030",  # 14. TODO
                "030, 030, 030",  # 15. TODO
                "030, 030, 030",  # 16. TODO
                "030, 030, 030",  # 17. TODO
                "030, 030, 030",  # 18. TODO
                "030, 030, 030",  # 19. TODO
                "030, 030, 030"  # 20. TODO
            ]
        ],
        [  # operator
            [  # bg
                "255, 255, 255",  # 0. white
                "025, 025, 025",  # 1. dark-grey
                "025, 025, 025",  # 2. TODO
                "025, 025, 025",  # 3. TODO
                "025, 025, 025",  # 4. TODO
                "025, 025, 025",  # 5. TODO
                "025, 025, 025",  # 6. TODO
                "025, 025, 025",  # 7. TODO
                "025, 025, 025",  # 8. TODO
                "025, 025, 025",  # 9. TODO
                "025, 025, 025",  # 10. TODO
                "025, 025, 025",  # 11. TODO
                "025, 025, 025",  # 12. TODO
                "025, 025, 025",  # 13. TODO
                "025, 025, 025",  # 14. TODO
                "025, 025, 025",  # 15. TODO
                "025, 025, 025",  # 16. TODO
                "025, 025, 025",  # 17. TODO
                "025, 025, 025",  # 18. TODO
                "025, 025, 025",  # 19. TODO
                "025, 025, 025"  # 20. TODO
            ],
            [  # border
                "000, 000, 255",  # 0. blue
                "000, 000, 255",  # 1. blue
                "000, 000, 255",  # 2. TODO
                "000, 000, 255",  # 3. TODO
                "000, 000, 255",  # 4. TODO
                "000, 000, 255",  # 5. TODO
                "000, 000, 255",  # 6. TODO
                "000, 000, 255",  # 7. TODO
                "000, 000, 255",  # 8. TODO
                "000, 000, 255",  # 9. TODO
                "000, 000, 255",  # 10. TODO
                "000, 000, 255",  # 11. TODO
                "000, 000, 255",  # 12. TODO
                "000, 000, 255",  # 13. TODO
                "149, 219, 229",  # 14. blue
                "238, 178, 056",  # 15. orange
                "255, 221, 226",  # 16. pink
                "147, 056, 095",  # 17. purple
                "211, 208, 203",  # 18. grey
                "219, 176, 074",  # 19. orange
                "237, 037, 078"  # 20. red
            ],
            [  # text
                "000, 000, 255",  # 0. blue
                "000, 000, 255",  # 1. blue
                "000, 000, 255",  # 2. TODO
                "000, 000, 255",  # 3. TODO
                "000, 000, 255",  # 4. TODO
                "000, 000, 255",  # 5. TODO
                "000, 000, 255",  # 6. TODO
                "000, 000, 255",  # 7. TODO
                "000, 000, 255",  # 8. TODO
                "000, 000, 255",  # 9. TODO
                "000, 000, 255",  # 10. TODO
                "000, 000, 255",  # 11. TODO
                "000, 000, 255",  # 12. TODO
                "000, 000, 255",  # 13. TODO
                "149, 219, 229",  # 14. blue
                "238, 178, 056",  # 15. orange
                "255, 221, 226",  # 16. pink
                "147, 056, 095",  # 17. purple
                "211, 208, 203",  # 18. grey
                "219, 176, 074",  # 19. orange
                "237, 037, 078"  # 20. red
            ],
            [  # pressed bg
                "200, 200, 200",  # 0. light-grey
                "040, 040, 040",  # 1. dark-grey
                "040, 040, 040",  # 2. TODO
                "040, 040, 040",  # 3. TODO
                "040, 040, 040",  # 4. TODO
                "040, 040, 040",  # 5. TODO
                "040, 040, 040",  # 6. TODO
                "040, 040, 040",  # 7. TODO
                "040, 040, 040",  # 8. TODO
                "040, 040, 040",  # 9. TODO
                "040, 040, 040",  # 10. TODO
                "040, 040, 040",  # 11. TODO
                "040, 040, 040",  # 12. TODO
                "040, 040, 040",  # 13. TODO
                "040, 040, 040",  # 14. TODO
                "040, 040, 040",  # 15. TODO
                "040, 040, 040",  # 16. TODO
                "040, 040, 040",  # 17. TODO
                "040, 040, 040",  # 18. TODO
                "040, 040, 040",  # 19. TODO
                "040, 040, 040"  # 20. TODO
            ]
        ],
        [  # display
            [  # bg
                "255, 255, 255",  # 0. white
                "040, 040, 040",  # 1. dark-grey
                "040, 040, 040",  # 2. TODO
                "040, 040, 040",  # 3. TODO
                "040, 040, 040",  # 4. TODO
                "040, 040, 040",  # 5. TODO
                "040, 040, 040",  # 6. TODO
                "040, 040, 040",  # 7. TODO
                "040, 040, 040",  # 8. TODO
                "040, 040, 040",  # 9. TODO
                "040, 040, 040",  # 10. TODO
                "040, 040, 040",  # 11. TODO
                "040, 040, 040",  # 12. TODO
                "040, 040, 040",  # 13. TODO
                "040, 040, 040",  # 14. TODO
                "040, 040, 040",  # 15. TODO
                "252, 246, 245",  # 16. white
                "040, 040, 040",  # 17. TODO
                "040, 040, 040",  # 18. TODO
                "040, 040, 040",  # 19. TODO
                "040, 040, 040"  # 20. TODO
            ],
            [  # text
                "000, 000, 000",  # 0. black
                "255, 255, 255",  # 1. white
                "255, 255, 255",  # 2. TODO
                "255, 255, 255",  # 3. TODO
                "255, 255, 255",  # 4. TODO
                "255, 255, 255",  # 5. TODO
                "255, 255, 255",  # 6. TODO
                "255, 255, 255",  # 7. TODO
                "255, 255, 255",  # 8. TODO
                "255, 255, 255",  # 9. TODO
                "255, 255, 255",  # 10. TODO
                "255, 255, 255",  # 11. TODO
                "255, 255, 255",  # 12. TODO
                "255, 255, 255",  # 13. TODO
                "255, 255, 255",  # 14. TODO
                "255, 255, 255",  # 15. TODO
                "016, 024, 032",  # 16. black
                "255, 255, 255",  # 17. TODO
                "255, 255, 255",  # 18. TODO
                "255, 255, 255",  # 19. TODO
                "255, 255, 255"  # 20. TODO
            ]
        ],
        [  # history
            [  # bg
                "255, 255, 255",  # 0. white
                "020, 020, 020",  # 1. dark-grey
                "020, 020, 020",  # 2. TODO
                "020, 020, 020",  # 3. TODO
                "020, 020, 020",  # 4. TODO
                "020, 020, 020",  # 5. TODO
                "020, 020, 020",  # 6. TODO
                "020, 020, 020",  # 7. TODO
                "020, 020, 020",  # 8. TODO
                "020, 020, 020",  # 9. TODO
                "020, 020, 020",  # 10. TODO
                "020, 020, 020",  # 11. TODO
                "020, 020, 020",  # 12. TODO
                "020, 020, 020",  # 13. TODO
                "020, 020, 020",  # 14. TODO
                "020, 020, 020",  # 15. TODO
                "020, 020, 020",  # 16. TODO
                "020, 020, 020",  # 17. TODO
                "020, 020, 020",  # 18. TODO
                "020, 020, 020",  # 19. TODO
                "020, 020, 020"  # 20. TODO
            ],
            [  # text
                "000, 000, 000",  # 0. black
                "255, 255, 255",  # 1. white
                "255, 255, 255",  # 2. TODO
                "255, 255, 255",  # 3. TODO
                "255, 255, 255",  # 4. TODO
                "255, 255, 255",  # 5. TODO
                "255, 255, 255",  # 6. TODO
                "255, 255, 255",  # 7. TODO
                "255, 255, 255",  # 8. TODO
                "255, 255, 255",  # 9. TODO
                "255, 255, 255",  # 10. TODO
                "255, 255, 255",  # 11. TODO
                "255, 255, 255",  # 12. TODO
                "255, 255, 255",  # 13. TODO
                "255, 255, 255",  # 14. TODO
                "255, 255, 255",  # 15. TODO
                "255, 255, 255",  # 16. TODO
                "255, 255, 255",  # 17. TODO
                "255, 255, 255",  # 18. TODO
                "255, 255, 255",  # 19. TODO
                "255, 255, 255"  # 20. TODO
            ]  # text
        ]  # history
    ]  # color palettes

    @staticmethod
    def change(index, main_window):
        """
        Changes colors of main window

        Args:
            index:   indexes currently used color scheme.
            main_window: references window, onto which new colors are going to be applied.

        """
        win_col = Palette.win_style[0:23] + Palette.colors[0][0][index] + \
                  Palette.win_style[36:51] + Palette.colors[0][1][index] + \
                  Palette.win_style[64:]
        main_window.setStyleSheet(win_col)

        num_col = Palette.num_but_style[0:35] + Palette.colors[1][0][index] + \
                  Palette.num_but_style[48:130] + Palette.colors[1][1][index] + \
                  Palette.num_but_style[143:157] + Palette.colors[1][2][index] + \
                  Palette.num_but_style[170:287] + Palette.colors[1][3][index] + \
                  Palette.num_but_style[300:]
        for but_id in [5, 6, 7, 10, 11, 12, 15, 16, 17, 20, 21, 22]:
            main_window.buttons[but_id].setStyleSheet(num_col)

        ctrl_col = Palette.ctrl_but_style[0:35] + Palette.colors[2][0][index] + \
                   Palette.ctrl_but_style[48:130] + Palette.colors[2][1][index] + \
                   Palette.ctrl_but_style[143:157] + Palette.colors[2][2][index] + \
                   Palette.ctrl_but_style[170:287] + Palette.colors[2][3][index] + \
                   Palette.ctrl_but_style[300:]
        for but_id in [8, 9]:
            main_window.buttons[but_id].setStyleSheet(ctrl_col)

        op_col = Palette.op_but_style[0:35] + Palette.colors[3][0][index] + \
                 Palette.op_but_style[48:130] + Palette.colors[3][1][index] + \
                 Palette.op_but_style[143:157] + Palette.colors[3][2][index] + \
                 Palette.op_but_style[170:287] + Palette.colors[3][3][index] + \
                 Palette.op_but_style[300:]
        for but_id in [0, 1, 2, 3, 4, 13, 14, 18, 19, 23, 24]:
            main_window.buttons[but_id].setStyleSheet(op_col)

        dis_col = Palette.display_style[0:33] + Palette.colors[4][0][index] + \
                  Palette.display_style[46:155] + Palette.colors[4][1][index] + \
                  Palette.display_style[168:]
        main_window.display.setStyleSheet(dis_col)

        his_col = Palette.hist_style[0:33] + Palette.colors[5][0][index] + \
                  Palette.hist_style[46:155] + Palette.colors[5][1][index] + \
                  Palette.hist_style[168:]
        main_window.history.setStyleSheet(his_col)
