#include <array>
#include <fstream>
#include <iostream>
#include <gmpxx.h>

auto tuppers = [](const mpz_t& abscissa, const mpz_t& ordinate) -> const bool
{
    bool result = false;
    mpz_t ordinate_mod_seventeen;
    mpz_t seventeen_mul_abscissa;
    mpz_t exponent;
    mpz_t ordinate_div_seventeen;
    mpz_t expression;
    mpz_inits(ordinate_mod_seventeen, seventeen_mul_abscissa, 
        exponent, ordinate_div_seventeen, expression, nullptr);
    mpz_mod_ui(ordinate_mod_seventeen, ordinate, 17);
    mpz_mul_ui(seventeen_mul_abscissa, abscissa, 17);
    mpz_add(exponent, seventeen_mul_abscissa, ordinate_mod_seventeen);
    mpz_div_ui(ordinate_div_seventeen, ordinate, 17);
    mpz_ui_pow_ui(expression, 2, mpz_get_ui(exponent));
    mpz_div(expression, ordinate_div_seventeen, expression);
    mpz_mod_ui(expression, expression, 2);
    result = mpz_get_ui(expression) != 0 ? true : false;
    mpz_clears(ordinate_mod_seventeen, seventeen_mul_abscissa, 
        exponent, ordinate_div_seventeen, expression, nullptr);
    return result;
};

auto write_ppm(const std::array<std::array<bool, 106>, 17>& tupper_plot) -> const void
{
    auto scale = 16;
    auto width = 106 * scale;
    auto height = 17 * scale;
    auto plot_colour = 1;
    auto background_colour = 0;
    std::ofstream output_file("tuppers_plot.ppm");
    output_file << "P3" << "\n";
    output_file << width << " " <<  height << "\n";
    output_file << "1" << "\n";
    for(const auto& row : tupper_plot)
    {
        std::string line_buffer = "";
        for(const auto& value : row)
        {
            auto character = value == true ? plot_colour ^ 48 : background_colour ^ 48;
            for(auto counter = 0; counter < scale; counter++) { line_buffer.append(1, character)
                .append(1, ' ')
                .append(1, character)
                .append(1, ' ')
                .append(1, character)
                .append(1, ' '); }
        }
        line_buffer.append(1, '\n');
        for(auto counter = 0; counter < scale; counter++) { output_file << line_buffer; }
    }
    output_file.close();
}

auto main(int argc, char** argv) -> int
{
    mpz_t abscissa;
    mpz_init_set_ui(abscissa, 0);
    mpz_t ordinate;
    if(argc != 2)
    {
        mpz_init_set_str(ordinate, "960939379918958884971672962127852754715004339660129306651505519271702802395266424689642842174350718121267153782770623355993237280874144307891325963941337723487857735749823926629715517173716995165232890538221612403238855866184013235585136048828693337902491454229288667081096184496091705183454067827731551705405381627380967602565625016981482083418783163849115590225610003652351370343874461848378737238198224849863465033159410054974700593138339226497249461751545728366702369745461014655997933798537483143786841806593422227898388722980000748404719", 10);
    }
    else
    {
        mpz_init_set_str(ordinate, argv[1], 10);
    }
    std::array<std::array<bool, 106>, 17> tupper_plot;
    for(auto row = 0; row < 17; row++)
    {
        mpz_set_ui(abscissa, 105);
        for(auto col = 0; col < 106; col++)
        {
            auto result = tuppers(abscissa, ordinate);
            mpz_sub_ui(abscissa, abscissa, 1);
            tupper_plot[row][col] = result;
        }
        mpz_add_ui(ordinate, ordinate, 1);
    }
    write_ppm(tupper_plot);
    return 0;
}